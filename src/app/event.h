#pragma once

#include <optional>
#include <string>
#include <iostream>

namespace impl {
    class event {
    public:
        std::string time;
        int event_id;
        std::string data;
        std::optional<int> table_id;

    public:

        event() = default;

        event(const event &) = default;

        event(event &&) = default;

        event &operator=(const event &) = default;

        event &operator=(event &&) = default;

        event(std::string time, int event_id, std::string data);

        event(std::string time, int event_id, std::string data, int table_id);

        friend std::istream &operator>>(std::istream &in, event &e);

        friend std::ostream &operator<<(std::ostream &out, const event &e);
    };
}