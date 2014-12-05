cmake_minimum_required(VERSION 2.6.4)

SET( EX_PLATFORM 64 )
SET( EX_PLATFORM_NAME "x64" )

project(CacheTest)

include_directories(. ~/boost_1_56_0/)
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -stdlib=libc++")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pthread -std=c++11")
    set(CMAKE_C_COMPILER /home/y/bin64/gcc)
    set(CMAKE_CXX_COMPILER /home/y/bin64/g++)
endif()

add_executable(cache_line_test cache_line_test.cc)
add_executable(cache_line_test2 ./cache_lint_test2.cc)
target_link_libraries(cache_line_test gflags)
