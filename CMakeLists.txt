cmake_minimum_required(VERSION 3.28)

project(rmdev C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

option(RMDEV_USE_STM32CUBEMX "Will link stm32cubemx to emdevif" ON)

set(EMDEVIF_USE_STM32CUBEMX RMDEV_USE_STM32CUBEMX)
if (RMDEV_USE_STM32CUBEMX)
    enable_language(ASM)
endif ()
add_subdirectory(emdevif_home/emdevif)

add_subdirectory(modules/rmdev_math)
add_subdirectory(modules/rmdev_control_algorithm)
add_subdirectory(modules/rmdev_kinematic_solution)

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME} PUBLIC
    emdevif

    rmdev_math
    rmdev_control_algorithm
    rmdev_kinematic_solution
)

if (RMDEV_ENABLE_TESTS)
    add_subdirectory(test/unit_test)
endif ()
