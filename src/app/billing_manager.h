#pragma once

#include <vector>
#include <cstdint>
namespace impl {
    class billing_manager {
        std::vector<int> elapsed_time;
        std::vector<uint64_t> revenue;

    public:
        explicit billing_manager(int table_count);

        void charge_table(int table_id, int duration_minutes, int hourly_rate);

        const std::vector<int> &get_times() const;

        const std::vector<uint64_t> &get_revenues() const;
    };

}