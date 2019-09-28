cmake_minimum_required(VERSION
        3.10)
project(secs)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

add_executable(secs
        test/main.cpp
        include/collections/vector.hpp
        include/collections/impl/vector.inl
        include/collections/sparse_map.hpp
        include/collections/impl/sparse_map.inl
        include/collections/view.hpp
        include/collections/impl/view.inl
        include/utility.hpp
        include/utility/range.hpp
        include/utility/repeat.hpp
        include/utility/type_utils.hpp
        include/utility/types.hpp
    )
