cmake_minimum_required(VERSION 3.30)
project(sistemas_operacionais)

set(CMAKE_CXX_STANDARD 17)

add_executable(sistemas_operacionais main.cpp
        printer.cpp
        writer.cpp)
