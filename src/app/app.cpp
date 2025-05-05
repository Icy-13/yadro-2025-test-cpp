#include "app.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>

void handle_client_arrival(const impl::event &e, int current_time, impl::context &ctx) {
    if (current_time < ctx.open_time || current_time > ctx.close_time) {
        ctx.events.emplace_back(e.time, ERROR_ID, "NotOpenYet");
    } else {
        if (!ctx.tables.add_client(e.data)) {
            ctx.events.emplace_back(e.time, ERROR_ID, "YouShallNotPass");
        }
    }
}

int free_table_helper(const impl::event &e, int current_time, impl::context &ctx) {
    auto [table_id, duration] = ctx.tables.free_table(e.data, current_time);
    ctx.billing.charge_table(table_id, duration, ctx.hour_cost);
    return table_id;
}

void handle_table_acquire(const impl::event &e, int current_time, impl::context &ctx) {
    if (!ctx.tables.client_exists(e.data)) {
        ctx.events.emplace_back(e.time, ERROR_ID, "ClientUnknown");
        return;
    }
    if (ctx.tables.table_is_busy(*e.table_id)) {
        ctx.events.emplace_back(e.time, ERROR_ID, "PlaceIsBusy");
        return;
    }
    free_table_helper(e, current_time, ctx);
    ctx.tables.acquire_table(current_time, *e.table_id, e.data);
}

void queue_pop_helper(const impl::event &e, int current_time, impl::context &ctx, int table_id) {
    auto [next_client, success] = ctx.tables.queue_pop(current_time, table_id);
    if (success) {
        ctx.events.emplace_back(e.time, ACQUIRE_ID, next_client, table_id);
    }
}

void handle_client_waits(const impl::event &e, int current_time, impl::context &ctx) {
    if (ctx.tables.get_free_tables() > 0) {
        ctx.events.emplace_back(e.time, ERROR_ID, "ICanWaitNoLonger!");
        return;
    }
    if (ctx.tables.get_queue_size() >= ctx.tables_count) {
        ctx.events.emplace_back(e.time, LEAVE_ID, e.data);
        return;
    }
    int table_id = free_table_helper(e, current_time, ctx);
    ctx.tables.queue_add(e.data);
    queue_pop_helper(e, current_time, ctx, table_id);
}

void handle_client_departure(const impl::event &e, int current_time, impl::context &ctx) {
    if (!ctx.tables.client_exists(e.data)) {
        ctx.events.emplace_back(e.time, ERROR_ID, "ClientUnknown");
        return;
    }
    int table_id = free_table_helper(e, current_time, ctx);
    queue_pop_helper(e, current_time, ctx, table_id);
}

void initialize_handlers(std::unordered_map<int, handle_signature> &handlers) {
    handlers[1] = &handle_client_arrival;
    handlers[2] = &handle_table_acquire;
    handlers[3] = &handle_client_waits;
    handlers[4] = &handle_client_departure;
}

void unknown_id_handler(const impl::event &e) {
    std::cerr << "Unknown event_id: " << e.event_id
              << "\nIn event: " << e << std::endl;
}

bool parse_event(std::string &line, impl::event &current_event) {
    std::istringstream iss(line);
    if (!read(iss, current_event)) {
        return false;
    }
    if (current_event.event_id == 2) {
        int table_id;
        if (!impl::read(iss, table_id)) {
            return false;
        }
        current_event.table_id.emplace(table_id - 1);
    }
    return true;
}

void finalize_day(impl::context &ctx, const std::string &close_time_string) {
    std::vector<std::string> remaining_clients;
    ctx.tables.fill_client_names(remaining_clients);
    std::sort(remaining_clients.begin(), remaining_clients.end());
    for (const auto &client_name: remaining_clients) {
        auto [table_id, duration] = ctx.tables.free_table(client_name, ctx.close_time);
        ctx.billing.charge_table(table_id, duration, ctx.hour_cost);
        ctx.events.emplace_back(close_time_string, LEAVE_ID, client_name);
    }
}

void print_result(impl::context &ctx, const std::string &open_time_string, const std::string &close_time_string) {
    std::cout << open_time_string << "\n";
    for (const auto &event: ctx.events) {
        std::cout << event << "\n";
    }
    std::cout << close_time_string << "\n";
    const auto &elapsed_time = ctx.billing.get_times();
    const auto &revenue = ctx.billing.get_revenues();
    for (int i = 0; i < ctx.tables_count; i++) {
        std::cout << i + 1 << " " << revenue[i] << " " << impl::time_to_string(elapsed_time[i]) << "\n";
    }
}

int run_app(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_file>" << std::endl;
        return 1;
    }
    std::ifstream fin(argv[1]);
    if (!fin) {
        std::cerr << "Couldn't open provided file " << argv[1] << std::endl;
        return 1;
    }
    int tables_count;
    std::string open_time_string, close_time_string;
    if (!impl::read(fin, tables_count, open_time_string, close_time_string)) {
        return 1;
    }
    int open_time = impl::parse_time(open_time_string);
    int close_time = impl::parse_time(close_time_string);
    int hour_cost;
    if (!impl::read(fin, hour_cost)) {
        return 1;
    }

    std::vector<impl::event> events;
    impl::table_manager table_manager(tables_count);
    impl::billing_manager billing_manager(tables_count);
    impl::context context{events, table_manager,
                          billing_manager, open_time,
                          close_time, hour_cost, tables_count};

    std::unordered_map<int, handle_signature> handlers;
    initialize_handlers(handlers);

    std::string line;
    fin.get(); // skip newline
    while (std::getline(fin, line)) {
        if (!fin) {
            impl::print_file_input_err_message();
            return 1;
        }
        impl::event current_event;
        if (!parse_event(line, current_event) != 0) {
            return 1;
        }

        events.push_back(current_event);

        auto handle = handlers.find(current_event.event_id);
        if (handle != handlers.end()) {
            handle->second(current_event, impl::parse_time(current_event.time), context);
        } else {
            unknown_id_handler(current_event);
            return 1;
        }
    }

    finalize_day(context, close_time_string);
    print_result(context, open_time_string, close_time_string);
    return 0;
}