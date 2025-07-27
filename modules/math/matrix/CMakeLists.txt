cmake_minimum_required(VERSION 3.28)

project(rmdev_matrix CXX)

add_subdirectory(MatrixType)
add_subdirectory(ArmMatrix)

add_library(${PROJECT_NAME} OBJECT)

target_link_libraries(${PROJECT_NAME} PUBLIC rmdev_core rmdev_math rmdev_matrix_arm_matrix)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET rmdev_matrix_modules TYPE CXX_MODULES FILES
        Matrix.cppm
)
