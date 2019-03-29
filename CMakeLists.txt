cmake_minimum_required(VERSION 3.10)
project(secs)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

add_executable(secs
        test/main.cpp
        include/engine.cpp
        include/engine.hpp
        include/entity_manager.cpp
        include/entity_manager.hpp
        include/component.cpp
        include/component.hpp
        include/utility.hpp
        include/entity.cpp
        include/entity.hpp
        include/event_manager.cpp
        include/event_manager.hpp
        include/callback_chain.cpp
        include/callback_chain.hpp
        include/object_pool.cpp
        include/object_pool.hpp)
