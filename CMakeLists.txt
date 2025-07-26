cmake_minimum_required(VERSION 3.28)

project(rmdev C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_subdirectory(inc)

add_library(${PROJECT_NAME} STATIC)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti -fmodules-ts -fno-exceptions -fno-rtti>
)

target_link_libraries(${PROJECT_NAME} PUBLIC rmdev_public_headers)

option(RMDEV_USE_STM32CUBEMX "Will link stm32cubemx lib to rmdev" OFF)
option(RMDEV_USE_CMSIS_DSP "Will link CMSIS-DSP lib to rmdev" OFF)

file(GLOB_RECURSE RMDEV_EXPORTED_MODULES "modules/*.cppm")
target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET rmdev_modules TYPE CXX_MODULES FILES
            ${RMDEV_EXPORTED_MODULES}
)

add_subdirectory(modules)
target_link_libraries(${PROJECT_NAME} PUBLIC
    rmdev__src_dir_resources
    rmdev__system_dir_resources
    rmdev_error_handler
    rmdev_control_algorithm
    rmdev_kinematic_solution
    rmdev_connectivity
    rmdev_thread
    rmdev_math
)

if (RMDEV_ENABLE_TESTS)
    add_subdirectory(test/unit_test)
endif ()
