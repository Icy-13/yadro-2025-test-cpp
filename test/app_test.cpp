#include <gtest/gtest.h>

#include "app.h"

#include <iostream>
#include <fstream>
#include <filesystem>

std::string read_file(const std::string &path) {
    std::ifstream file(path);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

class stream_redirection_wrapper {
public:
    stream_redirection_wrapper(std::ostream &stream, std::ostream &target)
            : stream_(stream), old_buf_(stream.rdbuf(target.rdbuf())) {}

    ~stream_redirection_wrapper() {
        stream_.rdbuf(old_buf_);
    }

private:
    std::ostream &stream_;
    std::streambuf *old_buf_;
};

void test_app(const std::string &input_path, const std::string &output_path) {
    std::stringstream cout_buffer, cerr_buffer;
    stream_redirection_wrapper redirect_cout(std::cout, cout_buffer);
    stream_redirection_wrapper redirect_cerr(std::cerr, cerr_buffer);

    std::string arg0 = "test";
    std::string arg1 = input_path;

    char *argv[] = {
            arg0.data(),
            arg1.data()
    };
    int exit_code = run_app(2, argv);

    EXPECT_EQ(exit_code, 0);
    EXPECT_TRUE(cerr_buffer.str().empty()) << cerr_buffer.str();

    std::string expected_result = read_file(output_path);
    std::string actual_result = cout_buffer.str();
    EXPECT_EQ(expected_result, actual_result);
}

TEST(app_test, sample) {
    test_app("inputs/sample.in", "outputs/sample.out");
}

TEST(app_test, single_table) {
    test_app("inputs/single_table.in", "outputs/single_table.out");
}

TEST(app_test, queue) {
    test_app("inputs/queue.in", "outputs/queue.out");
}