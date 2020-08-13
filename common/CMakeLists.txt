cmake_minimum_required(VERSION 3.15)
project(io-wrapper)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES io-wrapper.h io-wrapper.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})