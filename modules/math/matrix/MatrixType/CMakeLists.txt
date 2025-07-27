cmake_minimum_required(VERSION 3.28)

project(rmdev__matrix_type CXX)

add_library(${PROJECT_NAME} OBJECT)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET rmdev__matrix_type_modules TYPE CXX_MODULES FILES
        MatrixType.cppm
)
