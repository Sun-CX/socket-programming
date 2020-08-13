cmake_minimum_required(VERSION 3.15)
project(17)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES tsd.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})