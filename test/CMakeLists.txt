cmake_minimum_required(VERSION 3.28)

project(rmdev_unit_test C CXX)

add_library(${PROJECT_NAME} STATIC)

block()
    file(GLOB_RECURSE rmdevUnitTestTestSuitsSrcs "test_suits/*.cpp")
    target_sources(${PROJECT_NAME} PRIVATE
        test_main.cpp

        ${rmdevUnitTestTestSuitsSrcs}
    )
endblock()

target_link_libraries(${PROJECT_NAME} PRIVATE rmdev emdevif_test_framework emdevif_unit_test)
