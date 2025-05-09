cmake_minimum_required(VERSION 3.28)

project(rmdev C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

include("cmake/gcc-arm-none-eabi.cmake")

add_compile_options(-fmodules-ts -fno-exceptions)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    inc/
)

file(GLOB_RECURSE PUBLIC_MODULES "modules/*.cppm")
file(GLOB_RECURSE PRIVATE_MODULES "private/modules/*.cppm")
file(GLOB_RECURSE SOURCES "private/src/*.cpp")
target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET public_modules TYPE CXX_MODULES FILES ${PUBLIC_MODULES}

    PRIVATE
        FILE_SET private_modules TYPE CXX_MODULES FILES ${PRIVATE_MODULES}

    PRIVATE
        ${SOURCES}
)
