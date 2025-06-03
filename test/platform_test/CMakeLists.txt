cmake_minimum_required(VERSION 3.28)

if (DEFINED RMDEV_PLATFORM_TEST_PLATFORM)
    if (RMDEV_PLATFORM_TEST_PLATFORM MATCHES "STM32F407")
        add_subdirectory(stm32f407)
    else ()
        message(WARNING "Unsupported platform for platform_test: ${RMDEV_PLATFORM_TEST_PLATFORM}. Skipping.")
    endif ()



else ()
    message(WARNING "RMDEV_PLATFORM_TEST_PLATFORM is not defined, so platform_test skipped. Please set it to a valid platform (e.g., STM32F407).")
endif ()
