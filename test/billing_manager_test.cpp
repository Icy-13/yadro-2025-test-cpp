#include <gtest/gtest.h>

#include "billing_manager.h"

TEST(billing_manager_test, billing_test) {
    using impl::billing_manager;

    billing_manager billing(3);

    billing.charge_table(0, 1, 100);
    billing.charge_table(0, 2, 100);
    billing.charge_table(0, 60, 100);
    billing.charge_table(0, 61, 100);

    billing.charge_table(1, 1, 71);
    billing.charge_table(2, 1439, INT_MAX);

    auto& revenue = billing.get_revenues();
    std::vector<uint64_t> expected_revenue = {100 * (1 + 1 + 1 + 2), 71, INT_MAX * 1LL * ((1439+59) / 60)};
    EXPECT_EQ(revenue, expected_revenue);

    auto& times = billing.get_times();
    std::vector<int> expected_times = {1 + 2 + 60 + 61, 1, 1439};
    EXPECT_EQ(times, expected_times);
}