cmake_minimum_required(VERSION 3.28)

project(rmdev_unit_test C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC)

file(GLOB_RECURSE RMDEV_UNIT_TEST_TEST_SUITS_SRCS "test_suits/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE
    test_main.cpp

    ${RMDEV_UNIT_TEST_TEST_SUITS_SRCS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE rmdev emdevif_test_framework emdevif_unit_test)
