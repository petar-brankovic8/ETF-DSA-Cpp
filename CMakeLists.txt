cmake_minimum_required(VERSION 4.0.2)

project(ETF-DSA-Cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ETF-DSA-Cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graph.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graph.cpp
)