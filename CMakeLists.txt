cmake_minimum_required(VERSION 3.10)
project(MPI_Word_Counter)

find_package(MPI REQUIRED)
  
set(CMAKE_CXX_STANDARD 11)

include_directories(${MPI_INCLUDE_PATH}) 

add_executable(MPI_Word_Counter main.cpp)
target_link_libraries(MPI_Word_Counter ${MPI_CXX_LIBRARIES})
   