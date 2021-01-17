set(SOURCES src/read_files.cpp src/process_files.cpp) # add source files here

include_directories(inc)

add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${SOURCES})

add_executable(${CMAKE_PROJECT_NAME} main.cpp ${SOURCES})

if(MINGW)
    target_link_libraries(${CMAKE_PROJECT_NAME} ws2_32)
endif()

find_package (Threads REQUIRED)
target_link_libraries (${CMAKE_PROJECT_NAME} Threads::Threads)

find_package(Boost 1.71.0 COMPONENTS program_options system filesystem thread REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} Boost::program_options Boost::system Boost::thread Boost::filesystem )