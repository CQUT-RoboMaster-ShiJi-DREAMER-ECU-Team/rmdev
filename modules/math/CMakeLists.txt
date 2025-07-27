cmake_minimum_required(VERSION 3.28)

project(rmdev_math CXX)

add_subdirectory(matrix)

add_library(${PROJECT_NAME} OBJECT)

target_link_libraries(${PROJECT_NAME} PUBLIC rmdev_core)

file(GLOB RMDEV_MATH_IMPL_MODULES "math_impl/*.cppm")
target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET rmdev_math_modules TYPE CXX_MODULES FILES
        math.cppm
        ${RMDEV_MATH_IMPL_MODULES}
)
