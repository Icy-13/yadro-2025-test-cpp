#include "billing_manager.h"

impl::billing_manager::billing_manager(int table_count) :
        elapsed_time(table_count),
        revenue(table_count) {}

void impl::billing_manager::charge_table(int table_id, int duration, int hour_cost) {
    if (table_id == -1) {
        return;
    }
    elapsed_time[table_id] += duration;
    revenue[table_id] += ((duration + 59) / 60) * hour_cost;
}

const std::vector<int> &impl::billing_manager::get_times() const {
    return elapsed_time;
}

const std::vector<uint64_t> &impl::billing_manager::get_revenues() const {
    return revenue;
}

