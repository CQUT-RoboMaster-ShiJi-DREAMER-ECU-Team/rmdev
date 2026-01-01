/**
 * @file test_main.cpp
 * @brief 测试主函数
 */

#include <array>
#include <utility>
#include <string_view>

#include "test_main.hpp"
#include "rmdev_test_list.hpp"

#ifndef RMDEV_TEST_IGNORE_NAME_LIST
#error \
    "Please define the macro `RMDEV_TEST_IGNORE_NAME_LIST'. If nothing to ignore, define it to {""} ."
#endif

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

extern "C" void rmdev_testEntry(void)
{
    for (const auto& [test_name, test_func] : rmdev_test_list) {
        if (!inIgnoreNameList(test_name)) {
            if (test_func != nullptr) {
                test_func();
            }
        }
    }
}
