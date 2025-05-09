cmake_minimum_required(VERSION 3.28)

project(rmdev C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_compile_options(-fmodules-ts)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME} PUBLIC
        Inc/
)

file(GLOB_RECURSE RMDEV_LIB_MODULES "lib/*.cppm")
file(GLOB_RECURSE RMDEV_CONNECTIVITY_MODULES "connectivity/*.cppm")
target_sources(${PROJECT_NAME}
        PUBLIC
        FILE_SET CXX_MODULES FILES
        ${RMDEV_LIB_MODULES}
        ${RMDEV_CONNECTIVITY_MODULES}
)
