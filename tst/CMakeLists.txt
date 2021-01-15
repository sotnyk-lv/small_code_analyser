set(BINARY ${CMAKE_PROJECT_NAME}_test)

set(SOURCES main.cpp tests/basic_tests.cpp) # add other test source files here

add_executable(${BINARY} ${SOURCES})
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)

add_test(NAME ${BINARY} COMMAND ${BINARY})