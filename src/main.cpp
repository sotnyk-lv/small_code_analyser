#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/program_options.hpp>

#include <thread>
#include <mutex>

#include "read_files.h"
#include "process_files.h"
#include "time_counting.h"
#include "json_saver.h"

int main(int argc, char **argv) {
    //                           <------------ program options ------------>
    namespace po = boost::program_options;

    std::string root_path;

    po::options_description visible("Supported options");
    visible.add_options()
            ("help,h", "Print this help message.")
            ("save,s",  po::value<std::string>(), "Save results to the file.")
            ("threads,t", po::value<int>(), "Number of working threads");

    po::options_description hidden("Hidden options");
    hidden.add_options()
            ("root_path", po::value(&root_path), "Path to the root folder.");

    po::positional_options_description p;
    p.add("root_path", -1);

    po::options_description all("All options");
    all.add(visible).add(hidden);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(all).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << "Usage:\n  small_code_analyser <path> [-s savefile]\n" << visible << std::endl;
        return EXIT_SUCCESS;
    }



    std::chrono::high_resolution_clock::time_point start_time, indexing_time, finish_time;
    start_time = get_current_time_fenced();

    //                        <------------ getting source files ------------>
    std::vector<std::string> files = get_source_files(root_path);

    std::cout << files.size() << std::endl;
    indexing_time = get_current_time_fenced();

    std::map<std::string, unsigned long long> results = {
            {"blank", 0},
            {"commented", 0},
            {"code", 0},
            {"all", 0}
    };

    int working_threads;
    if (vm.count("threads")) {
        working_threads = vm["threads"].as<int>();
    }
    else {
        working_threads = std::thread::hardware_concurrency();
    }

    std::cout << working_threads << std::endl;

    boost::asio::thread_pool pool(working_threads);
    std::mutex m;
    for (auto & file: files) {
        boost::asio::post(pool, boost::bind(count_lines, file, std::ref(m), std::ref(results)));
    }
    pool.join();

    for (auto &value:results) {
        std::cout << value.first << " " << value.second << std::endl;
    }



//    std::vector<std::thread> threads;
//    std::mutex m;
//    for (auto & file: files) {
//        threads.emplace_back(count_lines, file, std::ref(m), std::ref(results));
//    }
//    for(auto& thread : threads) {
//        thread.join();
//    }
//    for (auto &value:results) {
//        std::cout << value.first << " " << value.second << std::endl;
//    }

    finish_time = get_current_time_fenced();
    std::cout << to_us(indexing_time - start_time)  <<  std::endl;
    std::cout << to_us(finish_time - indexing_time)  <<  std::endl;
    std::cout << to_us(finish_time - start_time)  <<  std::endl;

    std::map<std::string, double> times = {
            {"parsing directories", to_us(indexing_time - start_time)},
            {"parsing files", to_us(finish_time - indexing_time)},
            {"full program", to_us(finish_time - start_time)}
    };

    if (vm.count("save")) {
        std::cout << "Saving results to: " << vm["save"].as<std::string>() << std::endl;
        save_json(vm["save"].as<std::string>(),root_path, working_threads, files.size(), results, times);
    }


    return 0;
}