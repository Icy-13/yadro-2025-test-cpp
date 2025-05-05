#include "club_utils.h"
#include <format>
#include <iostream>

int impl::parse_time(const std::string &time) {
    if (time.size() != 5 || time[2] != ':') {
        throw std::invalid_argument("Invalid format of time. Expected format: XX:XX");
    }
    int hours = std::stoi(time.substr(0, 2));
    int minutes = std::stoi(time.substr(3));
    if (hours >= 24 || minutes >= 60) {
        throw std::invalid_argument("Incorrect time provided");
    }
    if (hours < 0 || minutes < 0) {
        throw std::invalid_argument("Time must be positive");
    }
    return hours * 60 + minutes;
}

std::string impl::time_to_string(int time) {
    if (time < 0) {
        throw std::invalid_argument("Given int is negative");
    }
    int hours = time / 60;
    int minutes = time % 60;
    if (hours >= 24) {
        throw std::invalid_argument("Given int is too large to represent time");
    }
    return std::format("{:02d}:{:02d}", hours, minutes);
}

void impl::print_file_input_err_message() {
    std::cerr << "An error occurred during read of the file" << std::endl;
}

