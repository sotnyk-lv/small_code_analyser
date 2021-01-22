//
// Created by danyl on 1/21/2021.
//

#ifndef SMALL_CODE_ANALYSER_ANALYSERDATA_H
#define SMALL_CODE_ANALYSER_ANALYSERDATA_H

#include <iostream>
#include <fstream>
#include <string>
#include <set>

#include <boost/atomic.hpp>
#include <boost/json.hpp>


struct AnalyserData{
    std::string folder_path;
    std::set<std::string> extensions = {".cpp", ".c", ".hpp", ".h"};

    struct LinesData {
        boost::atomic<unsigned long long int> blank = 0;
        boost::atomic<unsigned long long int> commented = 0;
        boost::atomic<unsigned long long int> code = 0;
        boost::atomic<unsigned long long int> all = 0;
    } lines;

    unsigned int number_of_threads;
    boost::atomic<unsigned long long int> number_of_file_parsed = 0;
    double parsing_time = 0;
    boost::json::object json_obj;

public:
    void save_results(std::string savefile_path);

    void print_results();

    void form_json();
};


#endif //SMALL_CODE_ANALYSER_ANALYSERDATA_H
