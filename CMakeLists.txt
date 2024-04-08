cmake_minimum_required(VERSION 3.10)
project(MPI_Most_Common_Word)

find_package(MPI REQUIRED)

set(CMAKE_CXX_STANDARD 11)

include_directories(${MPI_INCLUDE_PATH})

add_executable(MPI_Most_Common_Word main.cpp)
target_link_libraries(MPI_Most_Common_Word ${MPI_CXX_LIBRARIES})
 