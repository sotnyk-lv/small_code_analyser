//
// Created by danyl on 1/19/2021.
//

#include "CodeAnalyser.h"

CodeAnalyser::CodeAnalyser() {
    data.folder_path = ".";
    data.number_of_threads = boost::thread::hardware_concurrency();
    pool = new boost::asio::thread_pool(data.number_of_threads);
}

CodeAnalyser::CodeAnalyser(std::string path) {
    data.folder_path = path;
    data.number_of_threads = boost::thread::hardware_concurrency();
    pool = new boost::asio::thread_pool(data.number_of_threads);
}

CodeAnalyser::CodeAnalyser(std::string path, unsigned int new_number_of_threads) {
    data.folder_path = path;
    data.number_of_threads = new_number_of_threads;
    pool = new boost::asio::thread_pool(new_number_of_threads);
}

CodeAnalyser::~CodeAnalyser() {
    if (pool != nullptr) {
        delete pool;
        pool = nullptr;
    }
}

void CodeAnalyser::set_threads_number(unsigned int new_number_of_threads) {
    data.number_of_threads = new_number_of_threads;
    delete pool;
    pool = new boost::asio::thread_pool(new_number_of_threads);
}

void CodeAnalyser::set_file_extension(std::set<std::string> & new_extensions) {
    data.extensions = new_extensions;
}

void CodeAnalyser::analyse_folders(std::string path) {
    data.folder_path = path;
    this->analyse_folders();
}

void CodeAnalyser::analyse_folders() {
    std::chrono::high_resolution_clock::time_point start_time, finish_time;
    start_time = get_current_time_fenced();
    boost::asio::post(*pool, [this]
    {
        this->process_folder(this->data.folder_path);
    });

    (*pool).join();
    finish_time = get_current_time_fenced();
    data.parsing_time = to_us(finish_time - start_time);

}

void CodeAnalyser::process_folder(boost::filesystem::path path) {

    for (const auto & entry : boost::filesystem::directory_iterator(path, boost::filesystem::directory_options::skip_permission_denied)) {
        if ( data.extensions.count( boost::filesystem::extension( entry.path().string() ) ) ) {
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

    ++data.number_of_file_parsed;

    boost::filesystem::ifstream file{path};
    std::string str;
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

    data.lines.blank += local_results["blank"];
    data.lines.code += local_results["code"];
    data.lines.commented += local_results["commented"];
    data.lines.all += local_results["all"];
}

void CodeAnalyser::process_file_mmap(boost::filesystem::path path) {

    std::map<std::string, unsigned long long> local_results = {
            {"blank", 0},
            {"commented", 0},
            {"code", 0},
            {"all", 0}
    };

    ++data.number_of_file_parsed;

//    unsigned long long
//        file_size = boost::filesystem::file_size(path),
//        max_size = static_cast<unsigned long long>(1e9/data.number_of_threads);
//    unsigned long file_part = 0;
//
//    std::string line;
//
//    while (file_size - file_part*max_size > 0) {
//        unsigned long long batch_size = std::min(file_size - file_part*max_size, max_size);
//        boost::iostreams::mapped_file_source file(path.string(), batch_size, file_part*max_size);
//        unsigned long long previous_nl = 0;
//        for (unsigned long long i = 0; i < batch_size; ++i) {
//            if (file.data()[i] == '\n') {
//                line = std::string(file.data()[previous_nl], file.data()[i+1]);
//            }
//        }
//
//        ++file_part;
//    }


    data.lines.blank += local_results["blank"];
    data.lines.code += local_results["code"];
    data.lines.commented += local_results["commented"];
    data.lines.all += local_results["all"];
}

std::map<std::string, unsigned long long> CodeAnalyser::get_results() {
    std::map<std::string, unsigned long long> results = {
            {"blank", data.lines.blank.value()},
            {"commented", data.lines.commented.value()},
            {"code", data.lines.code.value()},
            {"all", data.lines.all.value()}
    };
    return results;
}

unsigned long long int CodeAnalyser::get_number_of_files_processed() {
    return data.number_of_file_parsed;
}

void CodeAnalyser::print_results() {
    data.print_results();
}

void CodeAnalyser::save_results(std::string savefile_path) {
    data.save_results(savefile_path);
}