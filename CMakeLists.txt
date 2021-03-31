################################################################################
#                                     SECS                                     #
################################################################################

cmake_minimum_required(VERSION 3.10)
project(SECS)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external)


################################################################################
#                                   library                                    #
################################################################################

add_library(secs INTERFACE)
target_include_directories(secs INTERFACE include)
target_link_libraries(secs INTERFACE utl)
target_compile_features(secs INTERFACE cxx_std_17)


################################################################################
#                                    tests                                     #
################################################################################

add_executable(secs_tests
        test/src/main.cpp
)
target_include_directories(secs_tests PRIVATE test/include)
target_link_libraries(secs_tests secs)
target_compile_options(secs_tests PRIVATE -Wall -Wextra -pedantic)
