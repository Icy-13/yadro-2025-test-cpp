#pragma once

#include <vector>
#include <string>
#include <queue>
#include <unordered_map>

namespace impl {
    class table_manager {
        int free_tables_count;
        std::unordered_map<std::string, int> clients_to_table;
        std::vector<int> active_tables;
        std::queue<std::string> clients_queue;

    public:
        explicit table_manager(int tables_count);

        std::pair<int, int> free_table(
                const std::string &client_name,
                int current_time);

        void acquire_table(int current_time,
                           int table_id,
                           const std::string &next_client);

        bool add_client(const std::string &client_name);

        bool client_exists(const std::string &client) const;

        bool table_is_busy(int table_id) const;

        int get_free_tables() const;

        std::size_t get_queue_size() const;

        void queue_add(const std::string &client_name);

        std::pair<std::string, bool> queue_pop(int current_time, int table_id);

        void fill_client_names(std::vector<std::string> &remaining_clients);
    };
}