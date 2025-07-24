cmake_minimum_required(VERSION 3.28)

project(unit_test C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

if (DEFINED RMDEV_UNIT_TEST_PLATFORM)
    if (RMDEV_UNIT_TEST_PLATFORM MATCHES "stm32f407ighx")
        add_subdirectory(stm32f407ighx)
    else ()
        message(WARNING "Unsupported platform for unit_test: ${RMDEV_UNIT_TEST_PLATFORM}. Skipping.")

        unset(RMDEV_UNIT_TEST_PLATFORM)
    endif ()

    if (DEFINED RMDEV_UNIT_TEST_PLATFORM)
        message("Will run test on platform: ${RMDEV_UNIT_TEST_PLATFORM}.")

        add_subdirectory(rmdev_test_framework)

        add_library(${PROJECT_NAME} INTERFACE)

        target_sources(${PROJECT_NAME} INTERFACE
            test_main.cpp

            test_suits/matrix_test.cpp
            test_suits/InitOnce_test.cpp
        )

        target_link_libraries(${PROJECT_NAME} INTERFACE rmdev rmdev_test_framework)
    endif ()

else ()
    message(WARNING "RMDEV_UNIT_TEST_PLATFORM is not defined, so unit_test skipped. Please set it to a valid platform (e.g., stm32f407ighx).")
endif ()
