cmake_minimum_required(VERSION 3.17)
project(external_code)

set(CMAKE_CXX_STANDARD 20)

add_executable(external_code main.cpp library.cpp library.h menu.cpp menu.h)