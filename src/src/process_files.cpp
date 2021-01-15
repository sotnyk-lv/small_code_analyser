//
// Created by danyl on 1/14/2021.
//

#include "inc/process_files.h"

#include <iostream>

std::map<std::string, unsigned long long> count_lines(std::string path) {
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
            std::cout << str << std::endl;
            ++results["commented"];
            if (str.find("*/") == std::string::npos) {
                while (getline(file, str)) {
                    if (str.find("*/") == std::string::npos) {
                        ++results["commented"];
                        ++results["all"];
                        std::cout << str << std::endl;
                    } else {
                        ++results["commented"];
                        ++results["all"];
                        std::cout << str << std::endl;
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
    return results;
}


void count_lines(std::string path, CountLines counter) {
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
            std::cout << str << std::endl;
            ++results["commented"];
            if (str.find("*/") == std::string::npos) {
                while (getline(file, str)) {
                    if (str.find("*/") == std::string::npos) {
                        ++results["commented"];
                        ++results["all"];
                        std::cout << str << std::endl;
                    } else {
                        ++results["commented"];
                        ++results["all"];
                        std::cout << str << std::endl;
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
    counter.add_data(&results);

}