cmake_minimum_required(VERSION 3.28)

project(platform_test C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

if (DEFINED RMDEV_PLATFORM_TEST_PLATFORM)
    if (RMDEV_PLATFORM_TEST_PLATFORM MATCHES "stm32f407ighx")
        add_subdirectory(stm32f407ighx)
    else ()
        message(WARNING "Unsupported platform for platform_test: ${RMDEV_PLATFORM_TEST_PLATFORM}. Skipping.")

        unset(RMDEV_PLATFORM_TEST_PLATFORM)
    endif ()

    if (DEFINED RMDEV_PLATFORM_TEST_PLATFORM)
        message("Will run test on platform: ${RMDEV_PLATFORM_TEST_PLATFORM}.")

        add_subdirectory(rmdev_test_framework)

        add_library(${PROJECT_NAME} INTERFACE)

        target_sources(${PROJECT_NAME} INTERFACE
            test_main.cpp
        )

        target_link_libraries(${PROJECT_NAME} INTERFACE rmdev rmdev_test_framework)
    endif ()

else ()
    message(WARNING "RMDEV_PLATFORM_TEST_PLATFORM is not defined, so platform_test skipped. Please set it to a valid platform (e.g., STM32F407).")
endif ()
