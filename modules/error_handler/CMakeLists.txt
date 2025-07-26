cmake_minimum_required(VERSION 3.28)

project(rmdev_error_handler CXX)

add_library(${PROJECT_NAME} OBJECT)

target_link_libraries(${PROJECT_NAME} PUBLIC rmdev_public_headers)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET error_handler_module TYPE CXX_MODULES FILES error_handler.cppm
    PUBLIC error_handler.cpp
)
