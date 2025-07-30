/**
 * @file test_main.cpp
 * @author 杜以成
 * @date 2025-06-03
 * @brief 测试主函数
 */

#include "test_main.hpp"

extern "C" void emdevif_testEntry(void);

extern "C" void rmdev_testEntry(void)
{
    emdevif_testEntry();

    matrixTest();

    OmniWheelInvSolverTest();
}
