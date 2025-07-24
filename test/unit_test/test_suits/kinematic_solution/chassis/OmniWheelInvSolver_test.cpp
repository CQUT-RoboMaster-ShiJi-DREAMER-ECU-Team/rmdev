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
        auto result = chassis_solver.solve({.vx = 1.0f, .vy = 0.0f, .omega = 0.0f});
        FP_EXPECT_GT(result.fl, 0.0f);
        FP_EXPECT_GT(result.fr, 0.0f);
    }
    TEST_CASE_END();
}

void OmniWheelInvSolverTest() {}
