//
// Created by danyl on 1/14/2021.
//

#ifndef SMALL_CODE_ANALYSER_PROCESS_FILES_H
#define SMALL_CODE_ANALYSER_PROCESS_FILES_H

#include <map>
#include <string>
#include <boost/filesystem.hpp>

//#include <mutex>
#include <boost/thread/mutex.hpp>

#include "inc/CountLines.h"

//std::map<std::string, unsigned long long> CountLines(std::string path);
void count_lines(std::string path, CountLines &counter);
void count_lines(std::string path, std::mutex & m, std::map<std::string, unsigned long long> & file_data);

#endif //SMALL_CODE_ANALYSER_PROCESS_FILES_H
