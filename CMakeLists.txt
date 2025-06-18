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

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET rmdev_error_handler_module TYPE CXX_MODULES FILES
            modules/error_handler.cppm
            modules/util/InitOnce.cppm

    PUBLIC
        src/error_handler.cpp
)

if (NOT(DEFINED RMDEV_DISABLE_MATH))
    file(GLOB RMDEV_MATH_PARTS "src/util/math/*.cppm")
    target_sources(${PROJECT_NAME}
        PUBLIC
            FILE_SET rmdev_math_module TYPE CXX_MODULES FILES
                modules/util/math.cppm
                ${RMDEV_MATH_PARTS}
    )
endif ()

if (NOT(DEFINED RMDEV_DISABLE_MATRIX))
    if (DEFINED RMDEV_DISABLE_MATH)
        message(FATAL_ERROR "Matrix module requires math module, but you disabled it!")
    endif ()

    file(GLOB_RECURSE RMDEV_MATRIX_MODULES
        "src/util/Matrix/*.cppm"
        "modules/util/Matrix/*.cppm"
    )
    target_sources(${PROJECT_NAME}
        PUBLIC
            FILE_SET rmdev_matrix_module TYPE CXX_MODULES FILES
                modules/util/Matrix.cppm
                ${RMDEV_MATRIX_MODULES}
    )
endif ()

if (NOT(DEFINED RMDEV_DISABLE_THREAD))
    file(GLOB_RECURSE RMDEV_THREAD_MODULES
        "System/*.cppm"
        "modules/thread/*.cppm"
    )
    target_sources(${PROJECT_NAME}
        PUBLIC
            FILE_SET rmdev_thread_module TYPE CXX_MODULES FILES
                ${RMDEV_THREAD_MODULES}
    )
endif ()

if (NOT(DEFINED RMDEV_DISABLE_CONTROL_ALGORITHM))
    if (DEFINED RMDEV_DISABLE_MATH)
        message(FATAL_ERROR "Control algorithm module requires math module, but you disabled it!")
    endif ()

    file(GLOB_RECURSE RMDEV_CONTROL_ALGORITHM_MODULES
        "modules/control_algorithm/*.cppm"
    )
    target_sources(${PROJECT_NAME}
        PUBLIC
            FILE_SET rmdev_control_algorithm_module TYPE CXX_MODULES FILES
                ${RMDEV_CONTROL_ALGORITHM_MODULES}
    )
endif ()

if (NOT(DEFINED RMDEV_DISABLE_KINEMATIC_SOLUTION))
    if (DEFINED RMDEV_DISABLE_MATRIX)
        message(FATAL_ERROR "Kinematic solution module requires matrix module, but you disabled it!")
    endif ()

    file(GLOB_RECURSE RMDEV_KINEMATIC_SOLUTION_MODULES
        "modules/kinematic_solution/*.cppm"
    )
    target_sources(${PROJECT_NAME}
        PUBLIC
            FILE_SET rmdev_kinematic_solution_module TYPE CXX_MODULES FILES
                ${RMDEV_KINEMATIC_SOLUTION_MODULES}
    )
endif ()

if (NOT(DEFINED RMDEV_DISABLE_CONNECTIVITY))
    file(GLOB_RECURSE RMDEV_CONNECTIVITY_MODULES
        "modules/connectivity/*.cppm"
    )
    target_sources(${PROJECT_NAME}
        PUBLIC
            FILE_SET rmdev_connectivity_module TYPE CXX_MODULES FILES
                ${RMDEV_CONNECTIVITY_MODULES}
    )
endif ()

if (RMDEV_ENABLE_TESTS)
    add_subdirectory(test/unit_test)
endif ()
