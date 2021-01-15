//
// Created by danyl on 1/14/2021.
//

#ifndef SMALL_CODE_ANALYSER_READ_FILES_H
#define SMALL_CODE_ANALYSER_READ_FILES_H

#include <boost/filesystem.hpp>
#include <vector>
#include <string>
#include <set>

std::vector<std::string> get_source_files(
        std::string & root_path,
        std::set<std::string> extension_masks = {".cpp", ".c", ".hpp", ".h"}
        );

#endif //SMALL_CODE_ANALYSER_READ_FILES_H
