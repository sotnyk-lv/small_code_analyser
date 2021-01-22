# small_code_analyser

This is code analyser that can parse a project and calculate the number of lines, number of lines with code, blank lines and commented lines of source files. 
By default it's reading all the ```.c``` ```.cpp``` ```.h``` ```.hpp``` files. This values can be changed by method set_file_extension() or by command line interface.
The project support parralel computing.


## Author

- [Sahaidak Danylo](https://github.com/sotnyk-lv)

## Prerequisites

 - **C++ compiler** - needs to support **C++17** standard
 - **CMake** 3.17+
 - **boost 1.75.0+**

## Usage

```bash
small_code_analyser <path> [-s/--save <json_savefile>] [-t/--threads <number_of_threads>] [-e/--extensions <extension_1> .. <extension_n>]
```

Help flags `-h`|`--help` support is available.
