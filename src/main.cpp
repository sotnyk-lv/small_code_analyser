#include <iostream>

#include <boost/program_options.hpp>

#include "CodeAnalyser.h"


int main(int argc, char **argv) {
    //                           <------------ program options ------------>
    namespace po = boost::program_options;

    std::string root_path;

    po::options_description visible("Supported options");
    visible.add_options()
            ("help,h", "Print this help message.")
            ("save,s",  po::value<std::string>(), "Save results to the file.")
            ("threads,t", po::value<int>(), "Number of working threads");

    po::options_description hidden("Hidden options");
    hidden.add_options()
            ("root_path", po::value(&root_path), "Path to the root folder.");

    po::positional_options_description p;
    p.add("root_path", -1);

    po::options_description all("All options");
    all.add(visible).add(hidden);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(all).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << "Usage:\n  small_code_analyser <path> [-s/-save savefile] [-t/-threads number_of_threads]\n" << visible << std::endl;
        return EXIT_SUCCESS;
    }

    unsigned int number_of_active_threads;
    if (vm.count("threads")) {
        number_of_active_threads = vm["threads"].as<int>();
    }
    else {
        number_of_active_threads = boost::thread::hardware_concurrency();
    }

    CodeAnalyser analyzer(root_path, number_of_active_threads);

    analyzer.analyse_folders();
    analyzer.print_results();

    if (vm.count("save")) {
        analyzer.save_results(vm["save"].as<std::string>());
    }

    return 0;
}