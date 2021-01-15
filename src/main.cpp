#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/thread_pool.hpp>

#include <thread>
#include <mutex>

#include "inc/read_files.h"
#include "inc/process_files.h"
#include "inc/CountLines.h"
#include "time_counting.h"

void cout_path(std::string path, std::mutex &m) {
    std::lock_guard s(m);
    std::cout << path << std::endl;
}

int main() {

//    commented code
    std::string root_path = "../.."; //some comment

    std::vector<std::string> files = get_source_files(root_path);

    std::cout << files.size() << std::endl;

    std::map<std::string, unsigned long long> results = {
            {"blank", 0},
            {"commented", 0},
            {"code", 0},
            {"all", 0}
    };

    CountLines counter;

    boost::asio::thread_pool pool(4);
    std::mutex m;
    for (auto & file: files) {
//        boost::asio::post(pool, boost::bind(count_lines(file, std::ref(counter))));
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
//        threads.emplace_back(cout_path, file, std::ref(m));
//    }
//    for(auto& thread : threads) {
//        thread.join();
//    }

    return 0;
}