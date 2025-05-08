cmake_minimum_required(VERSION 3.28)

project(rmdev C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(rmdev STATIC)

file(GLOB_RECURSE RMDEV_LIB_SOURCES "lib/*.*")
target_sources(rmdev PRIVATE
    ${RMDEV_LIB_SOURCES}
)
