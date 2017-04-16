cmake_minimum_required(VERSION 3.7)
project(HelloTemplate)

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-g -O2")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_SOURCE_DIR}/build/bin)

set(SOURCE_FILES main.cpp)
add_executable(HelloTemplate ${SOURCE_FILES})

add_subdirectory(socket_server)
add_subdirectory(socket_client)