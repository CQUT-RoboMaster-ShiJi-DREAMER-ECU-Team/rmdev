cmake_minimum_required(VERSION 3.28)

project(rmdev C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_library(${PROJECT_NAME} STATIC)

option(RMDEV_USE_STM32CUBEMX "Will link stm32cubemx lib to rmdev" OFF)
option(RMDEV_USE_CMSIS_DSP "Will link CMSIS-DSP lib to rmdev" OFF)

if (RMDEV_USE_STM32CUBEMX)
    enable_language(ASM)
endif ()

add_subdirectory(rmdev_core)
add_subdirectory(modules)
add_subdirectory(support/cmsis_dsp_support)
add_subdirectory(support/stm32cubemx_support)
target_link_libraries(${PROJECT_NAME} PUBLIC rmdev_core)
target_link_libraries(${PROJECT_NAME} PUBLIC
    rmdev_control_algorithm
    rmdev_kinematic_solution
    rmdev_connectivity
    rmdev_thread
    rmdev_math
)

if (RMDEV_ENABLE_TESTS)
    add_subdirectory(test/unit_test)
endif ()
