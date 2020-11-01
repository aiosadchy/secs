cmake_minimum_required(VERSION 3.10)
project(SECS)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external)

set(LIBRARY_NAME ${PROJECT_NAME})
add_library(${LIBRARY_NAME} INTERFACE)
target_include_directories(${LIBRARY_NAME} INTERFACE include)
target_link_libraries(${LIBRARY_NAME} INTERFACE utl)
target_compile_features(${LIBRARY_NAME} INTERFACE cxx_std_17)

set(TEST_PROJECT_NAME ${PROJECT_NAME}_test)
add_executable(${TEST_PROJECT_NAME}
        test/main.cpp
)
target_link_libraries(${TEST_PROJECT_NAME} ${LIBRARY_NAME})
target_compile_options(${TEST_PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
