//
// Created by danyl on 1/15/2021.
//

#ifndef SMALL_CODE_ANALYSER_COUNTLINES_H
#define SMALL_CODE_ANALYSER_COUNTLINES_H

#include <boost/thread/mutex.hpp>

class CountLines {
private:
    boost::mutex m;
    std::map<std::string, unsigned long long> results = {
            {"blank", 0},
            {"commented", 0},
            {"code", 0},
            {"all", 0}
    };

public:
    void get_result(std::map<std::string, unsigned long long> *res);
    void add_data(std::map<std::string, unsigned long long> *data);
};

#endif //SMALL_CODE_ANALYSER_COUNTLINES_H
