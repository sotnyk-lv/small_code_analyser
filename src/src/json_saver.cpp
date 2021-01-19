//
// Created by danyl on 1/18/2021.
//

#include "json_saver.h"

void save_json(
        std::string savefile,
        std::string &folder_path,
        int number_of_parsing_threads,
        unsigned long long int number_of_file_parsed,
        std::map<std::string, unsigned long long int> &line_values,
        std::map<std::string, double> &time_values) {

    boost::json::object obj;
    obj[ "folder parsed" ] = folder_path;
    obj[ "file parsed" ] = number_of_file_parsed;
    obj[ "number of threads" ] = number_of_parsing_threads;
    boost::json::array arr;
    arr.resize(line_values.size());
    int i = 0;
    for (auto &value: line_values) {
        arr[i].emplace_object()[value.first] = value.second;
        ++i;
    }
    obj[ "lines" ] = arr;
//    arr.clear();
    arr.resize(time_values.size());
    i = 0;
    for (auto &value: time_values) {
        arr[i].emplace_object()[value.first] = value.second;
        ++i;
    }
    obj[ "time" ] = arr;

    std::ofstream out(savefile);

    out << obj << std::endl;
}


//{
//    "folder parsed"      : "../..",
//    "file parsed"        : 96,
//    "number of threads"  : 4,
//    "lines" : [
//            {"all"       : 45032},
//            {"blank"     : 6081},
//            {"code"      : 24853},
//            {"commented" : 14098}
//    ],
//    "time":[
//            {"full program"        : 6.0942E-2},
//            {"parsing directories" : 1.7948E-2},
//            {"parsing files"       : 4.2993E-2}
//    ]
//}
