/**
 * @file OmniWheelInvSolver_test.cpp
 * @author DuYicheng
 * @date 2025-07-24
 * @brief 全向轮逆运动学解算测试
 */

#include "rmdev_test_framework.h"

import rmdev.kinematic_solution.chassis.OmniWheelInvSolver;

TEST_SUIT(FourWheelsOmniWheelInvSolverTest)
{
    const rmdev::OmniWheelInvSolver<float, 4> chassis_solver{0.5};

    TEST_CASE_BEGIN(StrightTest)
    {
        const auto result = chassis_solver.solve({.vx = 1.0f, .vy = 0.0f, .omega = 0.0f});
        FP_EXPECT_GT(result.fl, 0.0f);
        FP_EXPECT_GT(result.fr, 0.0f);
        FP_EXPECT_GT(result.bl, 0.0f);
        FP_EXPECT_GT(result.br, 0.0f);
        FP_EXPECT_EQ(result.fl, result.fr);
        FP_EXPECT_EQ(result.bl, result.fr);
        FP_EXPECT_EQ(result.br, result.fr);
    }
    TEST_CASE_END();
}

void OmniWheelInvSolverTest()
{
    RUN_SUIT(FourWheelsOmniWheelInvSolverTest);
}
