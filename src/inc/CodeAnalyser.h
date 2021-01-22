#ifndef SMALL_CODE_ANALYSER_CODEANALYSER_H
#define SMALL_CODE_ANALYSER_CODEANALYSER_H

#include <string>
#include <map>
#include <set>
#include <fstream>

#include <boost/asio/thread_pool.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/atomic.hpp>

#include <boost/iostreams/code_converter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

#include "time_counting.h"
#include "AnalyserData.h"

class CodeAnalyser {
public:
    CodeAnalyser();
    CodeAnalyser(std::string path);
    CodeAnalyser(std::string path, unsigned int number_of_threads);

    ~CodeAnalyser();

    // set number of threads available for calculation
    void set_threads_number(unsigned int number_of_threads);

    // set extensions of the files that need to bee parsed
    void set_file_extension(std::set<std::string> new_extensions);

    // start folder analysing folder
    void analyse_folders();
    void analyse_folders(std::string path);

    // change tracked extensions
    std::map<std::string, unsigned long long> get_results();

    // get the number of files tracked
    unsigned long long int get_number_of_files_processed();

    // print the result
    void print_results();

    // save the result as json file
    void save_results(std::string savefile_path);


private:
    boost::asio::thread_pool *pool = nullptr;
    AnalyserData data;

    // read all files from the folder;
    // if it is folder call itself in new thread;
    // if the extension is in the extensions set - call process_file in new thread
    void process_folder(boost::filesystem::path path);

    // read a file and calculate empty and commented lines as well as lines with code
    void process_file(boost::filesystem::path path);
};

#endif //SMALL_CODE_ANALYSER_CODEANALYSER_H
