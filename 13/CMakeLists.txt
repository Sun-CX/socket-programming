cmake_minimum_required(VERSION 3.15)
project(13)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES socket-pair.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
