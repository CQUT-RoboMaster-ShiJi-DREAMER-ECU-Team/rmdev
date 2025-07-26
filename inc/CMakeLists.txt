cmake_minimum_required(VERSION 3.28)

project(rmdev_public_headers C CXX)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE ./)
