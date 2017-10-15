cmake_minimum_required(VERSION 3.8)
project(Database)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")

set(SOURCE_FILES
        catch.hpp
        database_lib.h
        Tests.cpp)

add_executable(Database ${SOURCE_FILES})