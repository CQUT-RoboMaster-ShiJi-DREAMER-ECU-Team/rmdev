cmake_minimum_required(VERSION 3.28)

project(rmdev C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

option(RMDEV_USE_STM32CUBEMX "Will link stm32cubemx to rmdev" ON)

if (NOT (DEFINED RMDEV_THREAD_SYSTEM))
    set(RMDEV_THREAD_SYSTEM "FreeRTOS")
endif ()

set(EMDEVIF_THREAD_SYSTEM ${RMDEV_THREAD_SYSTEM})
set(EMDEVIF_USE_STM32CUBEMX ${RMDEV_USE_STM32CUBEMX})

if (${RMDEV_THREAD_SYSTEM} STREQUAL "FreeRTOS")
    option(RMDEV_FREERTOS_USE_STM32CUBEMX "Will use stm32cubemx generated FreeRTOS sources" ON)

    set(EMDEVIF_FREERTOS_USE_STM32CUBEMX ${RMDEV_FREERTOS_USE_STM32CUBEMX})
endif ()

if (RMDEV_USE_STM32CUBEMX)
    enable_language(ASM)
endif ()

if (RMDEV_ENABLE_TESTS)
    set(EMDEVIF_ENABLE_TESTS ON)
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
    enable_testing()
    add_subdirectory(test)
endif ()
