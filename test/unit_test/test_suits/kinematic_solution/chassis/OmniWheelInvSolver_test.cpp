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
    rmdev::OmniWheelInvSolver<float, 4> chassis_solver{0.5};

    TEST_CASE_BEGIN(StrightTest)
    {
        auto result = chassis_solver.solve({1.0f,0.0f, 0.0f});
    }TEST_CASE_END();
}

void OmniWheelInvSolverTest()
{

}
