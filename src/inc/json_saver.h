//
// Created by danyl on 1/18/2021.
//

#ifndef SMALL_CODE_ANALYSER_JSON_SAVER_H
#define SMALL_CODE_ANALYSER_JSON_SAVER_H

#include <iostream>
#include <fstream>
#include "boost/json.hpp"

void save_json(
        std::string savefile,
        std::string &folder_path,
        int number_of_parsing_threads,
        unsigned long long int number_of_file_parsed,
        std::map<std::string, unsigned long long int> &line_values,
        std::map<std::string, double> &time_values);

#endif //SMALL_CODE_ANALYSER_JSON_SAVER_H
