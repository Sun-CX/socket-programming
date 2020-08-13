cmake_minimum_required(VERSION 3.15)
project(07)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES echo-server.cpp timeout.h)

include_directories(${CMAKE_SOURCE_DIR}/common)
link_directories(${PROJECT_BINARY_DIR}/common)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} io-wrapper)
