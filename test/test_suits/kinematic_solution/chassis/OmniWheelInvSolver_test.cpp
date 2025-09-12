/**
 * @file OmniWheelInvSolver_test.cpp
 * @author DuYicheng
 * @date 2025-07-24
 * @brief 全向轮逆运动学解算测试
 */

#include "emdevif_test_framework.h"

import rmdev.kinematicSolution.chassis.OmniWheelInvSolver;

TEST_SUIT(FourWheelsOmniWheelInvSolverTest)
{
    const rmdev::OmniWheelInvSolver<float, 4> chassis_solver{0.5};

    TEST_CASE_BEGIN(StopTest)
    {
        const auto result = chassis_solver.solve({.vx = 0.0f, .vy = 0.0f, .omega = 0.0f});
        FP_EXPECT_EQ(0.0, result.fr);
        FP_EXPECT_EQ(result.fl, result.fr);
        FP_EXPECT_EQ(result.bl, result.fr);
        FP_EXPECT_EQ(result.br, result.fr);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(ForwardTest)
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

    TEST_CASE_BEGIN(BackTest)
    {
        const auto result = chassis_solver.solve({.vx = -1.0f, .vy = 0.0f, .omega = 0.0f});
        FP_EXPECT_LT(result.fl, 0.0f);
        FP_EXPECT_LT(result.fr, 0.0f);
        FP_EXPECT_LT(result.bl, 0.0f);
        FP_EXPECT_LT(result.br, 0.0f);
        FP_EXPECT_EQ(result.fl, result.fr);
        FP_EXPECT_EQ(result.bl, result.fr);
        FP_EXPECT_EQ(result.br, result.fr);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(LeftTest)
    {
        const auto result = chassis_solver.solve({.vx = 0.0f, .vy = 1.0f, .omega = 0.0f});
        FP_EXPECT_LT(result.fl, 0.0f);
        FP_EXPECT_GT(result.fr, 0.0f);
        FP_EXPECT_GT(result.bl, 0.0f);
        FP_EXPECT_LT(result.br, 0.0f);
        FP_EXPECT_EQ(-result.fl, result.fr);
        FP_EXPECT_EQ(result.bl, result.fr);
        FP_EXPECT_EQ(-result.br, result.fr);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(RightTest)
    {
        const auto result = chassis_solver.solve({.vx = 0.0f, .vy = -1.0f, .omega = 0.0f});
        FP_EXPECT_GT(result.fl, 0.0f);
        FP_EXPECT_LT(result.fr, 0.0f);
        FP_EXPECT_LT(result.bl, 0.0f);
        FP_EXPECT_GT(result.br, 0.0f);
        FP_EXPECT_EQ(-result.fl, result.fr);
        FP_EXPECT_EQ(result.bl, result.fr);
        FP_EXPECT_EQ(-result.br, result.fr);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(RotatingTest)
    {
        // 逆时针
        auto result = chassis_solver.solve({.vx = 0.0f, .vy = 0.0f, .omega = 1.0f});
        FP_EXPECT_LT(result.fl, 0.0f);
        FP_EXPECT_GT(result.fr, 0.0f);
        FP_EXPECT_LT(result.bl, 0.0f);
        FP_EXPECT_GT(result.br, 0.0f);
        FP_EXPECT_EQ(-result.fl, result.fr);
        FP_EXPECT_EQ(-result.bl, result.fr);
        FP_EXPECT_EQ(result.br, result.fr);

        // 顺时针
        result = chassis_solver.solve({.vx = 0.0f, .vy = 0.0f, .omega = -1.0f});
        FP_EXPECT_GT(result.fl, 0.0f);
        FP_EXPECT_LT(result.fr, 0.0f);
        FP_EXPECT_GT(result.bl, 0.0f);
        FP_EXPECT_LT(result.br, 0.0f);
        FP_EXPECT_EQ(-result.fl, result.fr);
        FP_EXPECT_EQ(-result.bl, result.fr);
        FP_EXPECT_EQ(result.br, result.fr);
    }
    TEST_CASE_END();
}

void OmniWheelInvSolverTest()
{
    RUN_SUIT(FourWheelsOmniWheelInvSolverTest);
}
