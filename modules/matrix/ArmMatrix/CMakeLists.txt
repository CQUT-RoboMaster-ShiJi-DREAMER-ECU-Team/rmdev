cmake_minimum_required(VERSION 3.28)

project(rmdev_matrix_arm_matrix CXX)

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME} PUBLIC rmdev_core rmdev_math rmdev_cmsis_dsp_support rmdev__matrix_type)

file(GLOB RMDEV_MATRIX_ARM_MATRIX_MODULES "./*.cppm")
target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET rmdev_matrix_arm_matrix_modules TYPE CXX_MODULES FILES
        ${RMDEV_MATRIX_ARM_MATRIX_MODULES}
)
