#include <iostream>

#include "inc/read_files.h"
#include "inc/process_files.h"
#include "inc/CountLines.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/thread_pool.hpp>

#include <thread>
#include <mutex>


void cout_path(std::string path, std::mutex &m) {
    std::lock_guard s(m);
    std::cout << path << std::endl;
}

int main() {

//    commented code
    std::string root_path = "..\\.."; //some comment

    std::vector<std::string> files = get_source_files(root_path);

    std::cout << files.size() << std::endl;

    std::map<std::string, unsigned long long> results = {
            {"blank", 0},
            {"commented", 0},
            {"code", 0},
            {"all", 0}
    };

    boost::asio::thread_pool pool(4);
    std::mutex m;
    for (auto & file: files) {
        boost::asio::post(pool, boost::bind(cout_path, file, std::ref(m)));
    }
    pool.join();

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