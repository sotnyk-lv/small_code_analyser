//
// Created by danyl on 1/19/2021.
//

#ifndef SMALL_CODE_ANALYSER_CODEANALYSER_H
#define SMALL_CODE_ANALYSER_CODEANALYSER_H

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <filesystem>
#include <fstream>

#include <boost/asio/thread_pool.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/json.hpp>
#include <boost/atomic.hpp>

#include "time_counting.h"


class CodeAnalyser {
public:
    CodeAnalyser();
    CodeAnalyser(std::string & path);
    CodeAnalyser(std::string & path, unsigned int number_of_threads);

    ~CodeAnalyser();

    void set_threads_number(unsigned int number_of_threads);

    void set_file_extension(std::set<std::string> new_extensions);

    void analyse_folders();

    std::map<std::string, unsigned long long> get_results();

    unsigned long long int get_number_of_files_processed();

    void print_results();

    void save_results(std::string savefile_path);


private:
    boost::asio::thread_pool *pool = nullptr;
    boost::mutex m;
    std::string folder_path;
    std::set<std::string> extensions = {".cpp", ".c", ".hpp", ".h"};
    std::map<std::string, unsigned long long> results = {
            {"blank", 0},
            {"commented", 0},
            {"code", 0},
            {"all", 0}
    };
    unsigned int number_of_threads;
    boost::atomic<unsigned long long int> number_of_file_parsed;
    double parsing_time;
    boost::json::object json_obj;

    // read all files from the folder;
    // if it is folder call itself in new thread;
    // if the extension is in our extensions set - call process_file in new thread
    void process_folder(boost::filesystem::path path);

    void process_file(boost::filesystem::path path);

    void form_json();
};


#endif //SMALL_CODE_ANALYSER_CODEANALYSER_H
