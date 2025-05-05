#include <gtest/gtest.h>
#include "club_utils.h"

#include <string>

TEST(util_time_test, parse_time_correctness) {
    using impl::parse_time;

    EXPECT_EQ(parse_time("00:00"), 0);

    EXPECT_EQ(parse_time( "23:59"), 1439);

    EXPECT_EQ(parse_time("12:43"), 763);

    EXPECT_EQ(parse_time("06:00"), 360);

    EXPECT_EQ(parse_time("00:32"), 32);

    EXPECT_EQ(parse_time("02:05"), 125);
}

TEST(util_time_test, parse_time_errors) {
    using impl::parse_time;

    EXPECT_THROW(parse_time("23-55"), std::invalid_argument);

    EXPECT_THROW(parse_time("003:55"), std::invalid_argument);

    EXPECT_THROW(parse_time("23550"), std::invalid_argument);

    EXPECT_THROW(parse_time("2:51"), std::invalid_argument);

    EXPECT_THROW(parse_time("02:5"), std::invalid_argument);

    EXPECT_THROW(parse_time("10:64"), std::invalid_argument);

    EXPECT_THROW(parse_time("25:00"), std::invalid_argument);
}

TEST(util_time_test, time_to_string_correctness) {
    using impl::time_to_string;

    EXPECT_EQ("00:00", time_to_string(0));

    EXPECT_EQ("23:59", time_to_string(1439));

    EXPECT_EQ("11:59", time_to_string(719));

    EXPECT_EQ("01:02", time_to_string(62));

    EXPECT_EQ("00:02", time_to_string(2));

    EXPECT_EQ("06:00", time_to_string(360));
}

TEST(util_time_test, time_to_string_errors) {
    using impl::time_to_string;

    EXPECT_THROW(time_to_string(-1), std::invalid_argument);

    EXPECT_THROW(time_to_string(1440), std::invalid_argument);

    EXPECT_THROW(time_to_string(10000), std::invalid_argument);

}