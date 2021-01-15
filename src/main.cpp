#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/thread_pool.hpp>

#include <thread>
#include <mutex>

#include "inc/read_files.h"
#include "inc/process_files.h"
#include "time_counting.h"

int main() {

//    commented code
    std::string root_path = "/home/sotnyk/sotnyk/"; //some comment
//    std::string root_path = "../..";
    std::chrono::high_resolution_clock::time_point start_time, indexing_time, finish_time;
    start_time = get_current_time_fenced();

    std::vector<std::string> files = get_source_files(root_path);

    std::cout << files.size() << std::endl;
    indexing_time = get_current_time_fenced();

    std::map<std::string, unsigned long long> results = {
            {"blank", 0},
            {"commented", 0},
            {"code", 0},
            {"all", 0}
    };

    boost::asio::thread_pool pool(1);
    std::mutex m;
    for (auto & file: files) {
        boost::asio::post(pool, boost::bind(count_lines, file, std::ref(m), std::ref(results)));
    }
    pool.join();

    for (auto &value:results) {
        std::cout << value.first << " " << value.second << std::endl;
    }



//    std::vector<std::thread> threads;
//
//    std::mutex m;
//
//    for (auto & file: files) {
//        threads.emplace_back(count_lines, file, std::ref(m), std::ref(results));
//    }
//    for(auto& thread : threads) {
//        thread.join();
//    }

    finish_time = get_current_time_fenced();
    std::cout << to_us(indexing_time - start_time)  <<  std::endl;
    std::cout << to_us(finish_time - indexing_time)  <<  std::endl;
    std::cout << to_us(finish_time - start_time)  <<  std::endl;

    return 0;
}