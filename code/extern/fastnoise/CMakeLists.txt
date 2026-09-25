cmake_minimum_required(VERSION 3.12)

project(fastnoise)

add_library(fastnoise INTERFACE)

target_include_directories(fastnoise INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
