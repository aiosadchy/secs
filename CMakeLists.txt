cmake_minimum_required(VERSION 3.10)
project(secs)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

add_executable(secs
        test/main.cpp
        test/object_pool_test.cpp
        include/parameter_pack.hpp
        include/type_id.hpp
        include/type_set.hpp
        include/utility.hpp
        include/engine.hpp
        include/engine.cpp
        include/entity_manager.hpp
        include/impl/entity_manager.inl
        include/component.hpp
        include/impl/component.inl
        include/entity.hpp
        include/impl/entity.inl
        include/event_manager.hpp
        include/impl/event_manager.inl
        include/callback_chain.hpp
        include/impl/callback_chain.inl
        include/object_pool.hpp
        include/impl/object_pool.inl)
