cmake_minimum_required(VERSION 3.5)

project(random_table LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(inc)
add_executable(random_table src/main.cpp inc/random_table.h src/random_table.cpp)
