#pragma once

#include <string>
#include <istream>

namespace impl {
    int parse_time(const std::string &time);

    std::string time_to_string(int time);

    void print_file_input_err_message();

    template<typename... Args>
    bool read(std::istream &in, Args &... args) {
        if (!(in >> ... >> args)) {
            print_file_input_err_message();
            return false;
        }
        return true;
    }
}

