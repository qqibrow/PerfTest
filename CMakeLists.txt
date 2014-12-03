cmake_minimum_required(VERSION 2.6.4)

SET( EX_PLATFORM 64 )
SET( EX_PLATFORM_NAME "x64" )

project(Perf)

include_directories(. ~/boost_1_56_0/)
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -stdlib=libc++")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pthread -std=c++11")
    set(CMAKE_C_COMPILER /home/y/bin64/gcc)
    set(CMAKE_CXX_COMPILER /home/y/bin64/g++)
endif()


set(SOURCE_FILES main.cpp countdown_latch.cc QueueTest.cc)
add_executable(Perf ${SOURCE_FILES})
target_link_libraries(Perf gflags)
