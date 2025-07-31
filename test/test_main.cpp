/**
 * @file test_main.cpp
 * @author 杜以成
 * @date 2025-06-03
 * @brief 测试主函数
 */

#include "test_main.hpp"
#include "emdevif_test_framework.h"

extern "C" void emdevif_testEntry(void);

extern "C" void rmdev_testEntry(void)
{
    emdevif_testEntry();
    TEST_LOG("emdevif test finished.");

    matrixTest();

    OmniWheelInvSolverTest();
}
