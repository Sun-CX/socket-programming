cmake_minimum_required(VERSION 3.15)
project(18)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES posix-sem.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})