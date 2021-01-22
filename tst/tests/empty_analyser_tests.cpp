#include "gtest/gtest.h"

#include "inc/CodeAnalyser.h"

#include <map>
#include <string>

#include <boost/format.hpp>
#include <boost/thread.hpp>

#include <fstream>
#include <streambuf>

TEST(empty_code_analyser, empty_code_analyser_get_results) {
    CodeAnalyser code_analyser;
    std::map<std::string, unsigned long long> real_res = {
            {"blank",     0},
            {"commented", 0},
            {"code",      0},
            {"all",       0}
    };

    ASSERT_EQ(real_res, code_analyser.get_results());
    ASSERT_EQ(0, code_analyser.get_number_of_files_processed());
}

TEST(empty_code_analyser, empty_code_analyser_print_results) {
    CodeAnalyser code_analyser;

    testing::internal::CaptureStdout();

    code_analyser.print_results();

    std::string output = testing::internal::GetCapturedStdout();
    std::string expected_print_res = (boost::format("folder parsed: .\n"
                                     "number of files parsed: 0\n"
                                     "number of threads: %d\n"
                                     "lines result:\n"
                                     "  all - 0\n"
                                     "  code - 0\n"
                                     "  blank - 0\n"
                                     "  commented - 0\n"
                                     "parsing time: 0s\n") % boost::thread::hardware_concurrency()).str();
    EXPECT_EQ(output, expected_print_res);

}

TEST(empty_code_analyser, empty_code_analyser_save_results) {
    CodeAnalyser code_analyser;

    std::string temp_file = "test_save.json";

    code_analyser.save_results(temp_file);

    std::ifstream t(temp_file);
    std::string expected_file_res = (boost::format(
            R"({"folder parsed":".","number of files parsed":0,"number of threads":%d,"lines":{"all":0,"code":0,"blank":0,"commented":0},"time":0E0}%c)")
                    % boost::thread::hardware_concurrency() % "\n").str();
    std::string file_res;

    t.seekg(0, std::ios::end);
    file_res.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    file_res.assign((std::istreambuf_iterator<char>(t)),
               std::istreambuf_iterator<char>());

    EXPECT_EQ(expected_file_res, file_res);
    t.close();

    EXPECT_TRUE(boost::filesystem::remove(temp_file.c_str()));


}