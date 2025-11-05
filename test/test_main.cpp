/**
 * @file test_main.cpp
 * @author 杜以成
 * @date 2025-06-03
 * @brief 测试主函数
 */

#include <array>
#include <utility>
#include <string_view>

#include "test_main.hpp"
#include "emdevif_test_framework.h"

#ifndef RMDEV_TEST_IGNORE_NAME_LIST
#define RMDEV_TEST_IGNORE_NAME_LIST {""}
#endif

#define TEST_LIST_ARG(func_name) #func_name, func_name

static constexpr auto rmdev_test_list = std::to_array<std::pair<std::string_view, void (*)()>>(
    {{TEST_LIST_ARG(matrixTest)}, {TEST_LIST_ARG(omniWheelInvSolverTest)}});

static constexpr auto rmdev_test_ignore_name_list = std::to_array<std::string_view>(RMDEV_TEST_IGNORE_NAME_LIST);

static constexpr bool inIgnoreNameList(const std::string_view& name) noexcept
{
    for (const auto& e : rmdev_test_ignore_name_list) {
        if (e == name) {
            return true;
        }
    }

    return false;
}

extern "C" void emdevif_testEntry(void);

extern "C" void rmdev_testEntry(void)
{
    emdevif_testEntry();
    TEST_LOG("emdevif test finished.");

    for (const auto& [test_name, test_func] : rmdev_test_list) {
        if (!inIgnoreNameList(test_name)) {
            test_func();
        }
    }
}
