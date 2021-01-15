//
// Created by danyl on 1/14/2021.
//

#ifndef SMALL_CODE_ANALYSER_PROCESS_FILES_H
#define SMALL_CODE_ANALYSER_PROCESS_FILES_H

#include <map>
#include <string>
#include <boost/filesystem.hpp>

#include <boost/thread/mutex.hpp>

void count_lines(std::string path, std::mutex & m, std::map<std::string, unsigned long long> & file_data);

#endif //SMALL_CODE_ANALYSER_PROCESS_FILES_H
