//
// Created by danyl on 1/15/2021.
//

#include "CountLines.h"

void CountLines::get_result(std::map<std::string, unsigned long long> *res) {
    boost::mutex::scoped_lock s(m);
    for (auto & value: results) {
        (*res)[value.first] += value.second;
    }
}

void CountLines::add_data(std::map<std::string, unsigned long long> *data) {
    boost::mutex::scoped_lock s(m);
    for (auto & value: *data) {
        (results)[value.first] += value.second;
    }
}