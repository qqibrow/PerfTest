cmake_minimum_required(VERSION 2.8.4)
project(Perf)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")

set(SOURCE_FILES main.cpp countdown_latch.cc)
add_executable(Perf ${SOURCE_FILES})