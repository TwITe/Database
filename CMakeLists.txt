cmake_minimum_required(VERSION 3.8)
project(Database)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES catch.hpp database.h tests.cpp)

add_executable(Database ${SOURCE_FILES})