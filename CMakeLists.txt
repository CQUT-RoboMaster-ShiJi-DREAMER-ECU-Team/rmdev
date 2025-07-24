cmake_minimum_required(VERSION 3.28)

project(rmdev C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_library(${PROJECT_NAME} STATIC)

if (NOT(${CMAKE_CXX_FLAGS} MATCHES "-fmodules-ts"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmodules-ts")
endif ()
if (NOT(${CMAKE_CXX_FLAGS} MATCHES "-fno-exceptions"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
endif ()
if (NOT(${CMAKE_CXX_FLAGS} MATCHES "-fno-rtti"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif ()

target_include_directories(${PROJECT_NAME}
    PUBLIC
        inc/
)

option(RMDEV_USE_STM32CUBEMX "Will link stm32cubemx lib to rmdev" OFF)
option(RMDEV_USE_CMSIS_DSP "Will link CMSIS-DSP lib to rmdev" OFF)

if (RMDEV_USE_STM32CUBEMX)
    enable_language(ASM)
    target_link_libraries(${PROJECT_NAME} PUBLIC stm32cubemx)
endif ()

if (RMDEV_USE_CMSIS_DSP)
    if (RMDEV_USE_STM32CUBEMX)
        # do nothing
    elseif ((DEFINED RMDEV_ARM_MATH_HEADER_DIR) AND
            (DEFINED RMDEV_CMSIS_DSP_LIB_DIR) AND
            (DEFINED RMDEV_CMSIS_DSP_LIB_NAME))
        target_include_directories(${PROJECT_NAME} PUBLIC ${RMDEV_ARM_MATH_HEADER_DIR})
        target_link_directories(${PROJECT_NAME} PUBLIC ${RMDEV_CMSIS_DSP_LIB_DIR})
        target_link_libraries(${PROJECT_NAME} PUBLIC :${RMDEV_CMSIS_DSP_LIB_NAME})
    else ()
        message(FATAL_ERROR "Use of CMSIS-DSP requires either RMDEV_USE_STM32CUBEMX to be enabled or RMDEV_ARM_MATH_HEADER_DIR, RMDEV_CMSIS_DSP_LIB_DIR, and RMDEV_CMSIS_DSP_LIB_NAME to be defined.")
    endif ()

    target_compile_definitions(${PROJECT_NAME} PRIVATE -DRMDEV_USE_CMSIS_DSP=1)
endif ()

file(GLOB_RECURSE RMDEV_MODULES "modules/*.cppm" "src/*.cppm" "System/*.cppm")
target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET rmdev_error_handler_module TYPE CXX_MODULES FILES
            ${RMDEV_MODULES}

    PUBLIC
        src/error_handler.cpp
)

if (RMDEV_ENABLE_TESTS)
    add_subdirectory(test/unit_test)
endif ()
