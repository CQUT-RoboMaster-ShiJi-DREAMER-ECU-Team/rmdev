cmake_minimum_required(VERSION 3.28)

project(rmdev_kinematic_solution CXX)

add_library(${PROJECT_NAME} OBJECT)

target_link_libraries(${PROJECT_NAME} PUBLIC rmdev_public_headers)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET rmdev_kinematic_solution_modules TYPE CXX_MODULES FILES
        chassis/FourWheelChassisSolver.cppm
        chassis/OmniWheelInvSolver.cppm
)
