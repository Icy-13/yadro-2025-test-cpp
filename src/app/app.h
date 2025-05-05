#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "event.h"
#include "club_utils.h"
#include "table_manager.h"
#include "billing_manager.h"

const int LEAVE_ID = 11;
const int ACQUIRE_ID = 12;
const int ERROR_ID = 13;

namespace impl {
    struct context {
        std::vector<impl::event> &events;
        impl::table_manager &tables;
        impl::billing_manager &billing;
        int open_time, close_time, hour_cost, tables_count;
    };
}

using handle_signature = void (*)(const impl::event &, int, impl::context &);

void handle_client_arrival(const impl::event &e, int current_time, impl::context &ctx);

void handle_table_acquire(const impl::event &e, int current_time, impl::context &ctx);

void handle_client_waits(const impl::event &e, int current_time, impl::context &ctx);

void handle_client_departure(const impl::event &e, int current_time, impl::context &ctx);

void initialize_handlers(std::unordered_map<int, handle_signature> &handlers);

void unknown_id_handler(const impl::event &e);

bool parse_event(std::string &line, impl::event &current_event);

void finalize_day(impl::context &ctx, const std::string &close_time_string);

void print_result(impl::context &ctx, const std::string &open_time_string, const std::string &close_time_string);

int run_app(int argc, char **argv);