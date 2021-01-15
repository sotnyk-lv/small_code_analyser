//
// Created by danyl on 1/14/2021.
//

#include "inc/read_files.h"

std::vector<std::string> get_source_files(std::string & root_path, std::set<std::string> extension_masks) {
    std::vector<std::string> files;
    for (const auto & entry : boost::filesystem::recursive_directory_iterator(root_path)) {
        if ( extension_masks.count(boost::filesystem::extension(entry.path()))) {
            files.push_back(entry.path().string());
        }

    }
    return files;
}
