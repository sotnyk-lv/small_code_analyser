#include "gtest/gtest.h"

#include "inc/CodeAnalyser.h"

#include <map>
#include <string>

#include <boost/format.hpp>
#include <boost/thread.hpp>

#include <fstream>
#include <streambuf>
#include <cstdio>

TEST(nested_folders_code_analyser, code_analyser_get_results) {
    CodeAnalyser code_analyser;

    code_analyser.analyse_folders("../../tst/test_samples/test_sample_2");

    std::map<std::string, unsigned long long> real_res = {
            {"blank",     27},
            {"commented", 12},
            {"code",      80},
            {"all",       119}
    };

    ASSERT_EQ(real_res, code_analyser.get_results());
    ASSERT_EQ(4, code_analyser.get_number_of_files_processed());
}

TEST(nested_folders_code_analyser, code_analyser_print_results) {
    CodeAnalyser code_analyser("../../tst/test_samples/test_sample_2", 8);
    code_analyser.analyse_folders();

    testing::internal::CaptureStdout();

    code_analyser.print_results();

    std::string output = testing::internal::GetCapturedStdout();
    std::string expected_print_res = "folder parsed: ../../tst/test_samples/test_sample_2\n"
                                     "number of files parsed: 4\n"
                                     "number of threads: 8\n"
                                     "lines result:\n"
                                     "  all - 119\n"
                                     "  code - 80\n"
                                     "  blank - 27\n"
                                     "  commented - 12\n"
                                     "parsing time: XXXEXs\n";

    output.find_last_of("parsing time:");
    EXPECT_EQ(
            output.substr(0, output.find("parsing time")),
            expected_print_res.substr(0, expected_print_res.find("parsing time"))
    );

}

TEST(nested_folders_code_analyser, code_analyser_save_results) {
    CodeAnalyser code_analyser("../../tst/test_samples/test_sample_2");
    code_analyser.set_threads_number(10);
    std::set<std::string> ext = {".txt", ".h"};
    code_analyser.set_file_extension(ext);
    code_analyser.analyse_folders();

    std::string temp_file = "test_save.json";

    code_analyser.save_results(temp_file);

    std::ifstream t(temp_file);
    std::string expected_file_res = (boost::format(
            R"({"folder parsed":"../../tst/test_samples/test_sample_2","number of files parsed":3,"number of threads":10,"lines":{"all":40,"code":21,"blank":9,"commented":10},"time":0E0}%c)") %  "\n").str();
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