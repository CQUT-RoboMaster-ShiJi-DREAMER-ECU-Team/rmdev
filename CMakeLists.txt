cmake_minimum_required(VERSION 3.28)

project(rmdev C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_compile_options(-fmodules-ts -fno-exceptions -fno-rtti)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        inc/
)

file(GLOB_RECURSE ${PROJECT_NAME}_PUBLIC_MODULES "modules/*.cppm")
file(GLOB_RECURSE ${PROJECT_NAME}_PUBLIC_SOURCES "src/*.cpp")
target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET ${PROJECT_NAME}_public_modules TYPE CXX_MODULES FILES ${${PROJECT_NAME}_PUBLIC_MODULES}

    PUBLIC
        ${${PROJECT_NAME}_PUBLIC_SOURCES}
)

include(FetchContent)

include("cmake/FetchETL.cmake")

target_link_libraries(${PROJECT_NAME} PUBLIC etl::etl)
