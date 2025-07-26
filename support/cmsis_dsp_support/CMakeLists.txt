cmake_minimum_required(VERSION 3.28)

project(rmdev_cmsis_dsp_support C)

add_library(${PROJECT_NAME} INTERFACE)

if (RMDEV_USE_CMSIS_DSP)
    if (RMDEV_USE_STM32CUBEMX)
        enable_language(ASM)
        target_link_libraries(${PROJECT_NAME} INTERFACE stm32cubemx)
    elseif ((DEFINED RMDEV_ARM_MATH_HEADER_DIR) AND
            (DEFINED RMDEV_CMSIS_DSP_LIB_DIR) AND
            (DEFINED RMDEV_CMSIS_DSP_LIB_NAME))
        target_include_directories(${PROJECT_NAME} INTERFACE ${RMDEV_ARM_MATH_HEADER_DIR})
        target_link_directories(${PROJECT_NAME} INTERFACE ${RMDEV_CMSIS_DSP_LIB_DIR})
        target_link_libraries(${PROJECT_NAME} INTERFACE :${RMDEV_CMSIS_DSP_LIB_NAME})
    else ()
        message(FATAL_ERROR "Use of CMSIS-DSP requires either RMDEV_USE_STM32CUBEMX to be enabled or RMDEV_ARM_MATH_HEADER_DIR, RMDEV_CMSIS_DSP_LIB_DIR, and RMDEV_CMSIS_DSP_LIB_NAME to be defined.")
    endif ()

    target_compile_definitions(${PROJECT_NAME} PRIVATE -DRMDEV_USE_CMSIS_DSP=1)
endif ()
