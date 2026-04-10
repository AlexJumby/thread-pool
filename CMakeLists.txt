cmake_minimum_required(VERSION 3.16)
project(thread_pool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(thread_pool
    src/thread-pool.cpp
)
target_include_directories(thread_pool PRIVATE include)
target_link_libraries(thread_pool Threads::Threads)