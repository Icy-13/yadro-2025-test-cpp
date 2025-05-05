#include <gtest/gtest.h>

#include "table_manager.h"

#include <algorithm>
#include <vector>
#include <string>


TEST(table_manager_test, add_client) {
    using impl::table_manager;

    table_manager tables(3);
    EXPECT_FALSE(tables.client_exists("c1"));
    EXPECT_EQ(tables.get_free_tables(), 3);
    EXPECT_EQ(tables.get_queue_size(), 0);

    EXPECT_TRUE(tables.add_client("c1"));
    EXPECT_TRUE(tables.client_exists("c1"));
    EXPECT_EQ(tables.get_free_tables(), 3);
    EXPECT_EQ(tables.get_queue_size(), 0);

    EXPECT_FALSE(tables.add_client("c1"));
    EXPECT_TRUE(tables.client_exists("c1"));
    EXPECT_EQ(tables.get_free_tables(), 3);
    EXPECT_EQ(tables.get_queue_size(), 0);
}

TEST(table_manager_test, acquire_and_free) {
    using impl::table_manager;

    table_manager tables(3);
    tables.acquire_table(0, 0, "c1");
    EXPECT_TRUE(tables.client_exists("c1"));
    EXPECT_TRUE(tables.table_is_busy(0));
    EXPECT_EQ(tables.get_free_tables(), 2);

    tables.acquire_table(1, 0, "c2");
    EXPECT_TRUE(tables.client_exists("c1"));
    EXPECT_TRUE(tables.client_exists("c2"));
    EXPECT_TRUE(tables.table_is_busy(0));
    EXPECT_EQ(tables.get_free_tables(), 2);

    auto [table_id, duration] = tables.free_table("c2", 2);
    EXPECT_EQ(table_id, 0);
    EXPECT_EQ(duration, 1);
    EXPECT_FALSE(tables.client_exists("c2"));
    EXPECT_FALSE(tables.table_is_busy(table_id));
    EXPECT_EQ(tables.get_free_tables(), 3);

    tables.acquire_table(0, 0, "c1");
    tables.acquire_table(0, 1, "c2");
    tables.acquire_table(0, 2, "c3");
    EXPECT_EQ(tables.get_free_tables(), 0);
    std::tie(table_id, duration) = tables.free_table("c2", 10);
    EXPECT_FALSE(tables.client_exists("c2"));
    EXPECT_EQ(table_id, 1);
    EXPECT_EQ(tables.get_free_tables(), 1);
    EXPECT_EQ(duration, 10);
    tables.acquire_table(100, 0, "c2");
    EXPECT_TRUE(tables.client_exists("c1"));
    EXPECT_EQ(tables.get_free_tables(), 1);
}

TEST(table_manager_test, queue) {
    using impl::table_manager;

    table_manager tables(3);

    EXPECT_FALSE(tables.queue_pop(0, 0).second);

    tables.queue_add("c1");
    tables.queue_add("c2");
    EXPECT_EQ(tables.get_queue_size(), 2);

    EXPECT_FALSE(tables.queue_pop(0, -1).second);

    auto [name, success] = tables.queue_pop(0, 0);
    EXPECT_EQ(name, "c1");
    EXPECT_TRUE(success);
    EXPECT_TRUE(tables.table_is_busy(0));
    EXPECT_EQ(tables.get_free_tables(), 2);
    EXPECT_EQ(tables.get_queue_size(), 1);

    std::tie(name, success) = tables.queue_pop(0, 1);
    EXPECT_EQ(name, "c2");
    EXPECT_TRUE(success);
    EXPECT_TRUE(tables.table_is_busy(1));
    EXPECT_EQ(tables.get_free_tables(), 1);
    EXPECT_EQ(tables.get_queue_size(), 0);

    EXPECT_FALSE(tables.queue_pop(0, 0).second);
}

TEST(table_manager_test, fill_client_names) {
    using impl::table_manager;

    table_manager tables(3);

    tables.acquire_table(0, 0, "c1");
    tables.acquire_table(0, 0, "c2");
    tables.acquire_table(0, 1, "c3");
    tables.acquire_table(0, 2, "c4");
    tables.queue_add("c5");
    tables.queue_add("c6");
    tables.free_table("c2", 0);
    tables.queue_pop(10, 0);

    std::vector<std::string> clients;
    tables.fill_client_names(clients);
    std::sort(clients.begin(), clients.end());

    std::vector<std::string> expected_clients = {"c1", "c3", "c4", "c5", "c6"};

    EXPECT_EQ(clients, expected_clients);
}