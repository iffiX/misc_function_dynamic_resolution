cmake_minimum_required(VERSION 3.5)
project(untitled)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O3")
include_directories(./)
include_directories(./lib/)
include_directories(./lib/fmt/include)
file(GLOB_RECURSE test_hash_files ./hash/*)
file(GLOB_RECURSE test_ptr_files ./ptr/*)
add_executable(test_hash ${test_hash_files})
add_executable(test_ptr ${test_ptr_files})

# if you would like to debug macros

#execute_process(COMMAND g++ ${CMAKE_SOURCE_DIR}/hash/test_hash.cpp
#                             -E -I${CMAKE_SOURCE_DIR}/lib/fmt/include
#                             -std=c++11 -o ${CMAKE_SOURCE_DIR}/out.cpp)
#execute_process(COMMAND g++ ${CMAKE_SOURCE_DIR}/ptr/test_ptr.cpp
#                             -E -I${CMAKE_SOURCE_DIR}/lib/fmt/include
#                             -std=c++11 -o ${CMAKE_SOURCE_DIR}/out.cpp)


