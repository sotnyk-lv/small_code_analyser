//
// Created by danyl on 1/21/2021.
//

#include "AnalyserData.h"

void AnalyserData::print_results() {
    std::cout << "folder parsed: " << folder_path << std::endl;
    std::cout << "number of files parsed: " << number_of_file_parsed << std::endl;
    std::cout << "number of threads: " << number_of_threads << std::endl;
    std::cout << "lines result:" << std::endl;
    std::cout << "  all - " << lines.all.value() << std::endl;
    std::cout << "  code - " << lines.code.value() << std::endl;
    std::cout << "  blank - " << lines.blank.value() << std::endl;
    std::cout << "  commented - " << lines.commented.value() << std::endl;
    std::cout << "parsing time: " << parsing_time << "s" << std::endl;
}

void AnalyserData::form_json() {
    json_obj[ "folder parsed" ] = folder_path;
    json_obj[ "number of files parsed" ] = number_of_file_parsed;
    json_obj[ "number of threads" ] = number_of_threads;

    boost::json::object lines_data;

    lines_data["all"] = lines.all.value();
    lines_data["code"] = lines.code.value();
    lines_data["blank"] = lines.blank.value();
    lines_data["commented"] = lines.commented.value();

    json_obj[ "lines" ] = lines_data;
    json_obj[ "time" ] = parsing_time;
}

void AnalyserData::save_results(std::string savefile_path) {
    if (json_obj.empty()) {
        this->form_json();
    }

    std::ofstream out(savefile_path);
    out << json_obj << std::endl;
}