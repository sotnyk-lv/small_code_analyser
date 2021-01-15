//
// Created by danyl on 1/14/2021.
//

#include "inc/process_files.h"

#include <iostream>

void count_lines(std::string path, std::mutex & m, std::map<std::string, unsigned long long> &file_data) {
    std::map<std::string, unsigned long long> results = {
            {"blank", 0},
            {"commented", 0},
            {"code", 0},
            {"all", 0}
    };

    boost::filesystem::ifstream file{path};
    std::string str;
    std::vector<std::string> filenames;
    while(getline(file, str)) {
        ++results["all"];
        if (str.find("//") != std::string::npos) {
            ++results["commented"];
//            if (!std::all_of(str.begin(),str.begin()+str.find("//"),isspace)) {         //  code + comment handling
//                ++results["code"];
//            }
        }
        else if (str.find("/*") != std::string::npos) {
            ++results["commented"];
            if (str.find("*/") == std::string::npos) {
                while (getline(file, str)) {
                    if (str.find("*/") == std::string::npos) {
                        ++results["commented"];
                        ++results["all"];
                    } else {
                        ++results["commented"];
                        ++results["all"];
                        break;
                    }
                }
            }
        }
        else if (str.empty()) {
            ++results["blank"];
        }
        else {
            ++results["code"];
        }
    }
    std::lock_guard g(m);
    for (auto &value:results) {
        (file_data)[value.first] += value.second;
    }

}