cmake_minimum_required(VERSION 2.6.4)

SET( EX_PLATFORM 64 )
SET( EX_PLATFORM_NAME "x64" )

project(Perf)
include_directories(include)
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -stdlib=libc++")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pthread -std=c++11")
endif()

AUX_SOURCE_DIRECTORY(src SOURCE_FILES)
add_executable(Perf ${SOURCE_FILES})
target_link_libraries(Perf gflags)
