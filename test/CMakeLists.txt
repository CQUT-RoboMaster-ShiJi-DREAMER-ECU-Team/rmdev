cmake_minimum_required(VERSION 3.28...4.0.2)

project(rmdev_unit_test C CXX)

set(RMDEV_TEST_IGNORE_NAME_LIST "" CACHE STRING "Set to ignore specific rmdev test. \
You can use \';\' to set multiple tests(eg: set it equals to \"matrixTest;omniWheelInvSolverTest\" \
will ignore matrixTest and omniWheelInvSolverTest)."
)

add_library(${PROJECT_NAME} STATIC)

emdevif_listToCCppStringTypeInitializerList(rmdevUnitTestIgnoreNameList "${RMDEV_TEST_IGNORE_NAME_LIST}")
message(STATUS "[${PROJECT_NAME}]: macro `RMDEV_TEST_IGNORE_NAME_LIST\' defined to ${rmdevUnitTestIgnoreNameList}")
target_compile_definitions(${PROJECT_NAME} PRIVATE
    RMDEV_TEST_IGNORE_NAME_LIST=${rmdevUnitTestIgnoreNameList}
)

file(GLOB_RECURSE rmdevUnitTestTestSuitsSrcs "test_suits/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE
    test_main.cpp

    ${rmdevUnitTestTestSuitsSrcs}
)

target_link_libraries(${PROJECT_NAME} PRIVATE rmdev emdevif_test_framework emdevif_test_heap_checker)
