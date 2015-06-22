cmake_minimum_required(VERSION 3.2)
project(trabalhoICC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    trab1.c)

add_executable(trabalhoICC ${SOURCE_FILES})