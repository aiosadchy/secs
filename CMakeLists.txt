cmake_minimum_required(VERSION 3.10)
project(secs)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

add_executable(secs
        test/main.cpp
    )

target_include_directories(secs PRIVATE include)
