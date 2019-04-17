cmake_minimum_required(VERSION 3.13)
project(Soduku)

set(CMAKE_CXX_STANDARD 14)

add_executable(Soduku main.cpp soduku.cpp soduku.h puzzle.cpp puzzle.h)