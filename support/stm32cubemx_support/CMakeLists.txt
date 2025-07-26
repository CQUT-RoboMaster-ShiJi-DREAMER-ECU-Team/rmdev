cmake_minimum_required(VERSION 3.28)

project(rmdev_stm32cubemx_support C CXX ASM)

add_library(${PROJECT_NAME} INTERFACE)

if (RMDEV_USE_STM32CUBEMX)
    target_link_libraries(${PROJECT_NAME} INTERFACE stm32cubemx)
endif ()
