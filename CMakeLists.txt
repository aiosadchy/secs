cmake_minimum_required(VERSION 3.10)
project(SECS)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

add_executable(SECS
        test/main.cpp
        include/SECS/collections/vector.hpp
        include/SECS/collections/impl/vector.inl
        include/SECS/collections/sparse_map.hpp
        include/SECS/collections/impl/sparse_map.inl
        include/SECS/collections/view.hpp
        include/SECS/collections/impl/view.inl
        include/SECS/utility.hpp
        include/SECS/utility/math.hpp
        include/SECS/utility/impl/math.inl
        include/SECS/utility/memory.hpp
        include/SECS/utility/impl/memory.inl
        include/SECS/utility/range.hpp
        include/SECS/utility/impl/range.inl
        include/SECS/utility/repeat.hpp
        include/SECS/utility/types.hpp
        include/SECS/engine.hpp
        include/SECS/entity.hpp
        include/SECS/entity_manager.hpp
        include/SECS/impl/engine.inl
        include/SECS/utility/type_id.hpp
        include/SECS/utility/impl/type_id.inl
        include/SECS/impl/entity_manager.inl
        include/SECS/impl/entity.inl)

target_include_directories(SECS PUBLIC include)
