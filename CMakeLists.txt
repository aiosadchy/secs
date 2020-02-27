cmake_minimum_required(VERSION 3.10)
project(SECS)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(SECS
        test/main.cpp
        SECS/collections/vector.hpp
        SECS/collections/impl/vector.inl
        SECS/collections/sparse_map.hpp
        SECS/collections/impl/sparse_map.inl
        SECS/collections/view.hpp
        SECS/collections/impl/view.inl
        SECS/utility.hpp
        SECS/utility/math.hpp
        SECS/utility/impl/math.inl
        SECS/utility/memory.hpp
        SECS/utility/impl/memory.inl
        SECS/utility/range.hpp
        SECS/utility/impl/range.inl
        SECS/utility/repeat.hpp
        SECS/utility/type_utils.hpp
        SECS/utility/types.hpp
        )
