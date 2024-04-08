cmake_minimum_required(VERSION 3.1)
project(WordCountOpenMP)

find_package(OpenMP REQUIRED)

set(CMAKE_CXX_STANDARD 11) 

add_executable(word_count_openmp word_count_openmp.cpp)

target_link_libraries(word_count_openmp PRIVATE OpenMP::OpenMP_CXX)
