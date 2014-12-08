cmake_minimum_required(VERSION 2.6.4)

SET( EX_PLATFORM 64 )
SET( EX_PLATFORM_NAME "x64" )

project(CacheTest)

add_definitions(-DALIGN_CACHE_LINE)
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -stdlib=libc++")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pthread -std=c++11 -Wl,--no-as-needed")
    set(CMAKE_CXX_COMPILER g++)
endif()

add_executable(cache_line_test cache_line_test.cc)
add_executable(cache_line_test2 cache_line_test2.cc)
