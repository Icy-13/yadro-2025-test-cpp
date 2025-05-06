#include "table_manager.h"

impl::table_manager::table_manager(int tables_count) :
        free_tables_count(tables_count),
        active_tables(tables_count, -1) {}

std::pair<int, int> impl::table_manager::free_table(const std::string &client_name, int current_time) {
    int table_id = clients_to_table[client_name];
    if (table_id == -1) {
        return {-1, -1};
    }
    int duration = current_time - active_tables[table_id];
    active_tables[table_id] = -1;
    free_tables_count++;
    remove_client(client_name);
    return {table_id, duration};
}

void impl::table_manager::acquire_table(int current_time, int table_id, const std::string &next_client) {
    if (active_tables[table_id] == -1) {
        free_tables_count--;
    }
    clients_to_table[next_client] = table_id;
    active_tables[table_id] = current_time;
}

bool impl::table_manager::add_client(const std::string &client_name) {
    return clients_to_table.insert({client_name, -1}).second;
}

bool impl::table_manager::remove_client(const std::string &client_name) {
    return clients_to_table.erase(client_name);
}


bool impl::table_manager::client_exists(const std::string &client) const {
    return clients_to_table.count(client) != 0;
}

bool impl::table_manager::table_is_busy(int table_id) const {
    return active_tables[table_id] != -1;
}

int impl::table_manager::get_free_tables() const {
    return free_tables_count;
}

std::size_t impl::table_manager::get_queue_size() const {
    return clients_queue.size();
}

void impl::table_manager::queue_add(const std::string &client_name) {
    add_client(client_name);
    clients_queue.push(client_name);
}

std::pair<std::string, bool> impl::table_manager::queue_pop(int current_time, int table_id) {
    if (clients_queue.empty() || table_id == -1) {
        return {"", false};
    }
    if (table_is_busy(table_id)) {
        return {"", false};
    }
    std::string next_client = clients_queue.front();
    clients_queue.pop();
    acquire_table(current_time, table_id, next_client);
    return {next_client, true};
}

void impl::table_manager::fill_client_names(std::vector<std::string> &remaining_clients) {
    for (const auto &[name, table]: clients_to_table) {
        remaining_clients.push_back(name);
    }
}
