//
// Created by danyl on 1/19/2021.
//

#include "CodeAnalyser.h"

CodeAnalyser::CodeAnalyser() {
    folder_path = ".";
    number_of_threads = boost::thread::hardware_concurrency();
    pool = new boost::asio::thread_pool(number_of_threads);
}

CodeAnalyser::CodeAnalyser(std::string & path) {
    folder_path = path;
    number_of_threads = boost::thread::hardware_concurrency();
    pool = new boost::asio::thread_pool(number_of_threads);
}

CodeAnalyser::CodeAnalyser(std::string & path, unsigned int new_number_of_threads) {
    folder_path = path;
    number_of_threads = new_number_of_threads;
    pool = new boost::asio::thread_pool(new_number_of_threads);
}

CodeAnalyser::~CodeAnalyser() {
    if (pool != nullptr) {
        delete pool;
        pool = nullptr;
    }
}

void CodeAnalyser::set_threads_number(unsigned int new_number_of_threads) {
    number_of_threads = new_number_of_threads;
    delete pool;
    pool = new boost::asio::thread_pool(new_number_of_threads);
}

void CodeAnalyser::set_file_extension(std::set<std::string> new_extensions) {
    extensions = new_extensions;
}

void CodeAnalyser::analyse_folders() {
    std::chrono::high_resolution_clock::time_point start_time, finish_time;
    start_time = get_current_time_fenced();
    boost::asio::post(*pool, [this]
    {
        this->process_folder(this->folder_path);
    });

    (*pool).join();
    finish_time = get_current_time_fenced();
    parsing_time = to_us(finish_time - start_time);

}

void CodeAnalyser::process_folder(boost::filesystem::path path) {

    for (const auto & entry : boost::filesystem::directory_iterator(path, boost::filesystem::directory_options::skip_permission_denied)) {
        if ( extensions.count( boost::filesystem::extension( entry.path().string() ) ) ) {
            boost::asio::post(*pool, [this, entry]
            {
                this->process_file(entry.path());
            });
        }
        else if (boost::filesystem::is_directory(entry)) {
            boost::asio::post(*pool, [this, entry]
            {
                this->process_folder(entry.path());
            });
        }
    }
}

void CodeAnalyser::process_file(boost::filesystem::path path) {

    std::map<std::string, unsigned long long> local_results = {
            {"blank", 0},
            {"commented", 0},
            {"code", 0},
            {"all", 0}
    };

    ++number_of_file_parsed;

    boost::filesystem::ifstream file{path};
    std::string str;
    std::vector<std::string> filenames;
    while(getline(file, str)) {
        ++local_results["all"];
        if (str.find("//") != std::string::npos) {
            ++local_results["commented"];
//            if (!std::all_of(str.begin(),str.begin()+str.find("//"),isspace)) {         //  code + comment handling
//                ++local_results["code"];
//            }
        }
        else if (str.find("/*") != std::string::npos) {
            ++local_results["commented"];
            if (str.find("*/") == std::string::npos) {
                while (getline(file, str)) {                   // TODO: if second while breaks at the end of the file
                    if (str.find("*/") == std::string::npos) {
                        ++local_results["commented"];
                        ++local_results["all"];
                    } else {
                        ++local_results["commented"];
                        ++local_results["all"];
                        break;
                    }
                }
            }
        }
        else if (str.empty()) {
            ++local_results["blank"];
        }
        else {
            ++local_results["code"];
        }
    }
    std::lock_guard g(m);
    for (auto &value : local_results) {
        results[value.first] += value.second;
    }
}

std::map<std::string, unsigned long long> CodeAnalyser::get_results() {
    return results;
}

unsigned long long int CodeAnalyser::get_number_of_files_processed() {
    return number_of_file_parsed;
}

void CodeAnalyser::print_results() {
    std::cout << "folder parsed: " << folder_path << std::endl;
    std::cout << "number of files parsed: " << number_of_file_parsed << std::endl;
    std::cout << "number of threads: " << number_of_threads << std::endl;
    std::cout << "lines result:" << std::endl;
    for (auto &value: results) {
        std::cout << "  " << value.first << " - " << value.second << std::endl;
    }
    std::cout << "parsing time: " << parsing_time << "s" << std::endl;
}

void CodeAnalyser::form_json() {
    json_obj[ "folder parsed" ] = folder_path;
    json_obj[ "number of files parsed" ] = number_of_file_parsed;
    json_obj[ "number of threads" ] = number_of_threads;
    boost::json::array arr;
    arr.resize(results.size());
    int i = 0;
    for (auto &value: results) {
        arr[i].emplace_object()[value.first] = value.second;
        ++i;
    }
    json_obj[ "lines" ] = arr;

    json_obj[ "time" ] = parsing_time;
}

void CodeAnalyser::save_results(std::string savefile_path) {
    if (json_obj.empty()) {
        this->form_json();
    }

    std::ofstream out(savefile_path);
    out << json_obj << std::endl;
}