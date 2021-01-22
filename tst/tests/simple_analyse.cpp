#include "gtest/gtest.h"

#include "inc/CodeAnalyser.h"

#include <map>
#include <string>

#include <boost/format.hpp>
#include <boost/thread.hpp>

#include <fstream>
#include <streambuf>
#include <cstdio>

TEST(simple_code_analyser, code_analyser_get_results) {
    CodeAnalyser code_analyser("../../tst/test_samples/test_sample_1");

    code_analyser.analyse_folders();

    std::map<std::string, unsigned long long> real_res = {
            {"blank",     13},
            {"commented", 5}, //4
            {"code",      40}, //41
            {"all",       58}
    };

    ASSERT_EQ(real_res, code_analyser.get_results());
    ASSERT_EQ(2, code_analyser.get_number_of_files_processed());
}

TEST(simple_code_analyser, code_analyser_print_results) {
    CodeAnalyser code_analyser("../../tst/test_samples/test_sample_1");
    code_analyser.analyse_folders();

    testing::internal::CaptureStdout();

    code_analyser.print_results();

    std::string output = testing::internal::GetCapturedStdout();
    std::string expected_print_res = (boost::format("folder parsed: ../../tst/test_samples/test_sample_1\n"
                                                    "number of files parsed: 2\n"
                                                    "number of threads: %d\n"
                                                    "lines result:\n"
                                                    "  all - 58\n"
                                                    "  code - 40\n"
                                                    "  blank - 13\n"
                                                    "  commented - 5\n"
                                                    "parsing time: XXXEXs\n") % boost::thread::hardware_concurrency()).str();

    output.find_last_of("parsing time:");
    EXPECT_EQ(
            output.substr(0, output.find("parsing time")),
            expected_print_res.substr(0, expected_print_res.find("parsing time"))
            );

}

TEST(simple_code_analyser, code_analyser_save_results) {
    CodeAnalyser code_analyser("../../tst/test_samples/test_sample_1");
    code_analyser.analyse_folders();

    std::string temp_file = "test_save.json";

    code_analyser.save_results(temp_file);

    std::ifstream t(temp_file);
    std::string expected_file_res = (boost::format(R"({"folder parsed":"../../tst/test_samples/test_sample_1","number of files parsed":2,"number of threads":%d,"lines":{"all":58,"code":40,"blank":13,"commented":5},"time":0E0}%c)") % boost::thread::hardware_concurrency() % "\n").str();
    std::string file_res;

    t.seekg(0, std::ios::end);
    file_res.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    file_res.assign((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    EXPECT_EQ(
            expected_file_res.substr(0, expected_file_res.find("time")),
            file_res.substr(0, file_res.find("time"))
    );

    t.close();

    EXPECT_TRUE(boost::filesystem::remove(temp_file.c_str()));


}