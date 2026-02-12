/**
 * @file rmdev_test_list.hpp
 * @brief
 */

#pragma once
#ifndef RMDEV_TEST_LIST_HPP
#define RMDEV_TEST_LIST_HPP

#include <array>
#include <utility>
#include <string_view>

#include "test_main.hpp"

#define TEST_LIST_ARG(func_name) #func_name, func_name

#ifndef USE_GENERATED_RMDEV_TEST_LIST

static constexpr auto rmdev_test_list =
    std::to_array<std::pair<std::string_view, void (*)()>>({{TEST_LIST_ARG(matrixTest)},
                                                            {TEST_LIST_ARG(omniWheelInvSolverTest)},
                                                            {TEST_LIST_ARG(subscriberTest)},
                                                            {TEST_LIST_ARG(topicTest)}});

#else  // !USE_GENERATED_RMDEV_TEST_LIST

#include "rmdev_test_list.generated.hpp"

#endif  // !USE_GENERATED_RMDEV_TEST_LIST

#endif  // !RMDEV_TEST_LIST_HPP
