/**
 * @file matrix_test.cpp
 * @author 杜以成
 * @date 2025-06-04
 * @brief 矩阵运算测试
 */

#include <array>
#include <algorithm>

#include "emdevif_test_framework.h"

#include "arm_math.h"

import rmdev.matrix;
import rmdev.math;

static TEST_SUIT(ArmMatCInterfaceTest)
{
    TEST_CASE_BEGIN(ArmMatrixCInterfaceTest);
    {
        const float mat1_origin_data[3 * 3] = {1, 2, 3, 4, 7, 6, 7, 8, 10};

        arm_matrix_instance_f32 mat1;
        float mat1_data[3 * 3];
        std::copy_n(mat1_origin_data, 3 * 3, mat1_data);
        arm_mat_init_f32(&mat1, 3, 3, mat1_data);

        arm_matrix_instance_f32 mat1_inv;
        float mat1_inv_data[3 * 3];
        arm_mat_init_f32(&mat1_inv, 3, 3, mat1_inv_data);

        const auto status = arm_mat_inverse_f32(&mat1, &mat1_inv);
        EXPECT_TRUE(status == ARM_MATH_SUCCESS);
        constexpr float mat1_inv_expected_data[3 * 3] =
            {-0.88f, -0.16f, 0.36f, -0.08f, 0.44f, -0.24f, 0.68f, -0.24f, 0.04f};
        EXPECT_TRUE(std::equal(mat1_inv.pData,
                                     mat1_inv.pData + 3 * 3,
                                     mat1_inv_expected_data,
                                     [](const float a, const float b) -> bool { return rmdev::weakEqu(a, b); }));
        // 需要注意的是 arm_mat_inverse 运算完成后会把待求逆的矩阵化为单位矩阵，因此计算完成后，原来的数据会丢失
        EXPECT_TRUE(!std::equal(mat1_inv.pData,
                                      mat1_inv.pData + 3 * 3,
                                      mat1_origin_data,
                                      [](const float a, const float b) -> bool { return rmdev::weakEqu(a, b); }));
    }
    TEST_CASE_END();
}

rmdev::ArmMatrix<float, 3, 3> global_mat1{1, 2, 3, 4, 5, 6, 7, 8, 9};
rmdev::ArmMatrix<float, 3, 3> global_mat2{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
constexpr rmdev::ArmMatrix<float, 3, 3> global_mat1_origin{1, 2, 3, 4, 5, 6, 7, 8, 9};

static TEST_SUIT(ArmMatrixBasicTest)
{
    using rmdev::weakEqu;

    TEST_CASE_BEGIN(ArmMatrixBasicTest);
    {
        EXPECT_TRUE(weakEqu(global_mat1(1, 1), 1.0f));
        EXPECT_TRUE(weakEqu(global_mat1(2, 2), 5.0f));
        EXPECT_TRUE(global_mat1 == global_mat2);

        global_mat1 = {9, 8, 7, 6, 5, 4, 3, 2, 1};
        global_mat2 = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};

        EXPECT_TRUE(weakEqu(global_mat1(1, 1), 9.0f) && weakEqu(global_mat1(2, 1), 6.0f));
        EXPECT_TRUE(global_mat1 == global_mat2);
        EXPECT_TRUE(global_mat1 != global_mat1_origin);

        EXPECT_TRUE(global_mat1.equ({9, 8, 7, 6, 5, 4, 3, 2, 1}));
        EXPECT_TRUE(global_mat1.equ({{9, 8, 7}, {6, 5, 4}, {3, 2, 1}}));
        EXPECT_TRUE(global_mat2.equ({9, 8, 7, 6, 5, 4, 3, 2, 1}, 0.01f));
        EXPECT_TRUE(global_mat2.equ({{9, 8, 7}, {6, 5, 4}, {3, 2, 1}}, 0.01f));
        // EXPECT_TRUE(global_mat1 == {9, 8, 7, 6, 5, 4, 3, 2, 1});
        // EXPECT_TRUE(global_mat1 != {1, 8, 7, 6, 5, 4, 3, 2, 1});  // 不知为何，这两种写法会报错

        static constexpr rmdev::ArmMatrix<float, 3, 3> sta_conexpr_mat{};
        global_mat1.clear();
        EXPECT_TRUE(global_mat1 == sta_conexpr_mat);

        rmdev::ArmMatrix<float, 3, 3> mat1;

        EXPECT_TRUE(weakEqu(mat1(1, 1), 0.0f));

        mat1(1, 1) = 1.0f;
        mat1(2, 1) = 2.0f;
        mat1(3, 3) = 3.0f;
        EXPECT_TRUE(weakEqu(mat1(1, 1), 1.0f) && weakEqu(mat1(2, 1), 2.0f) && weakEqu(mat1(3, 3), 3.0f));

        *mat1.at(3, 1) = 245.0f;
        *mat1.at(2, 3) = 123.0f;
        *mat1.at(1, 2) = 456.0f;
        EXPECT_TRUE(weakEqu(*mat1.at(3, 1), 245.0f) && weakEqu(*mat1.at(2, 3), 123.0f) &&
                          weakEqu(*mat1.at(1, 2), 456.0f));

        std::array nullnum_in_mat1{mat1.at(0, 0),
                                   mat1.at(0, 2),
                                   mat1.at(3, 4),
                                   mat1.at(-1, 2),
                                   mat1.at(4, 3),
                                   mat1.at(4, 4),
                                   mat1.at(114514, 1919810)};
        using nullnum_arr_in_mat1_t = decltype(nullnum_in_mat1);
        nullnum_arr_in_mat1_t nullnum_in_mat1_expected{};
        nullnum_in_mat1_expected.fill(nullptr);
        EXPECT_TRUE(nullnum_in_mat1 == nullnum_in_mat1_expected);

        rmdev::ArmMatrix<float, 3, 3> mat2{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
        rmdev::ArmMatrix<float, 3, 3> mat3{{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}, {7.0f, 8.0f, 9.0f}};
        EXPECT_TRUE(
            rmdev::weakEqu(mat2(1, 1), 1.0f) && rmdev::weakEqu(mat2(1, 2), 2.0f) && rmdev::weakEqu(mat2(1, 3), 3.0f) &&
            rmdev::weakEqu(mat2(2, 1), 4.0f) && rmdev::weakEqu(mat2(2, 2), 5.0f) && rmdev::weakEqu(mat2(2, 3), 6.0f) &&
            rmdev::weakEqu(mat2(3, 1), 7.0f) && rmdev::weakEqu(mat2(3, 2), 8.0f) && rmdev::weakEqu(mat2(3, 3), 9.0f));

        bool mat2_and_mat3_are_equal = true;
        for (std::size_t i = 1; i <= 3; ++i) {
            for (std::size_t j = 1; j <= 3; ++j) {
                if (!rmdev::weakEqu(mat2(i, j), mat3(i, j))) {
                    mat2_and_mat3_are_equal = false;
                    break;
                }
            }
            if (!mat2_and_mat3_are_equal) {
                break;
            }
        }
        EXPECT_TRUE(mat2_and_mat3_are_equal);
        EXPECT_TRUE(mat2 == mat3);
        EXPECT_TRUE(mat3 == mat2);
        EXPECT_TRUE(!(mat2 == mat1));
        EXPECT_TRUE(!(mat2 != mat3));
        EXPECT_TRUE(mat2 != mat1);

        mat1 = mat2;
        EXPECT_TRUE(mat1 == mat2);

        rmdev::ArmMatrix<float, 3, 3> mat4{mat1};
        EXPECT_TRUE(mat4 == mat1);

        rmdev::ArmMatrix<float, 3, 3> mat4_2(mat1);
        EXPECT_TRUE(mat4_2 == mat1);

        const float data_equ_to_mat2[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
        rmdev::ArmMatrix<float, 3, 3> mat5{data_equ_to_mat2};
        rmdev::ArmMatrix<float, 3, 3> mat5_2(data_equ_to_mat2);
        EXPECT_TRUE(mat5 == mat2);
        EXPECT_TRUE(mat5_2 == mat2);

        const float data_equ_to_mat3[3][3] = {{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}, {7.0f, 8.0f, 9.0f}};
        rmdev::ArmMatrix<float, 3, 3> mat6{data_equ_to_mat3};
        rmdev::ArmMatrix<float, 3, 3> mat6_2(data_equ_to_mat3);
        EXPECT_TRUE(mat6 == mat3);
        EXPECT_TRUE(mat6_2 == mat3);
        EXPECT_TRUE(mat5 == mat6);

        rmdev::ArmMatrix<float, 3, 3> mat7 = mat2;
        EXPECT_TRUE(mat7 == mat2);

        const rmdev::ArmMatrix<float, 3, 3> mat8_const{mat2};
        EXPECT_TRUE(mat8_const == mat2);

        bool mat2_and_mat8_are_equal = true;
        for (std::size_t i = 1; i <= 3; ++i) {
            for (std::size_t j = 1; j <= 3; ++j) {
                if (!weakEqu(mat2(i, j), mat8_const(i, j))) {
                    mat2_and_mat8_are_equal = false;
                    break;
                }
            }
            if (!mat2_and_mat8_are_equal) {
                break;
            }
        }
        EXPECT_TRUE(mat2_and_mat8_are_equal);

        bool mat2_and_mat8_are_equal_use_at = true;
        for (std::size_t i = 1; i <= 3; ++i) {
            for (std::size_t j = 1; j <= 3; ++j) {
                if (!weakEqu(*mat8_const.at(i, j), mat2(i, j))) {
                    mat2_and_mat8_are_equal_use_at = false;
                    break;
                }
            }
            if (!mat2_and_mat8_are_equal_use_at) {
                break;
            }
        }
        EXPECT_TRUE(mat2_and_mat8_are_equal_use_at);
    }
    TEST_CASE_END();
}

static TEST_SUIT(ArmMatrixSpecialConstructTest)
{
    TEST_CASE_BEGIN(ArmMatrixSpecialConstructTest);
    {
        // 方阵
        [EMDEVIF_TEST_LAMBDA_CAPTURE] {
            using SquareMatrix = rmdev::ArmMatrix<float, 3, 3>;

            const SquareMatrix zero_expected{0, 0, 0, 0, 0, 0, 0, 0, 0};

            SquareMatrix zero(rmdev::MatrixBase::Zero);
            SquareMatrix normal(rmdev::MatrixBase::Normal);
            EXPECT_TRUE(zero == zero_expected);
            EXPECT_TRUE(normal.equ(zero_expected));

            const SquareMatrix e_expected{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

            SquareMatrix e{rmdev::MatrixBase::E};
            EXPECT_TRUE(e == e_expected);

            const SquareMatrix one_expected{1, 1, 1, 1, 1, 1, 1, 1, 1};

            SquareMatrix one{rmdev::MatrixBase::One};
            EXPECT_TRUE(one_expected == one);
        }();

        // 非方阵
        [EMDEVIF_TEST_LAMBDA_CAPTURE] {
            using NotSquareMatrix = rmdev::ArmMatrix<float, 3, 2>;

            const NotSquareMatrix zero_expected{0, 0, 0, 0, 0, 0};

            NotSquareMatrix zero(rmdev::MatrixBase::Zero);
            NotSquareMatrix normal(rmdev::MatrixBase::Normal);
            EXPECT_TRUE(zero == zero_expected);
            EXPECT_TRUE(normal.equ(zero_expected));

            const NotSquareMatrix e_expected{zero_expected};

            NotSquareMatrix e{rmdev::MatrixBase::E};
            EXPECT_TRUE(e == e_expected);

            const NotSquareMatrix one_expected{1, 1, 1, 1, 1, 1};

            NotSquareMatrix one{rmdev::MatrixBase::One};
            EXPECT_TRUE(one_expected == one);
        }();
    }
    TEST_CASE_END();
}

static TEST_SUIT(ArmMatrixCalcTest)
{
    TEST_CASE_BEGIN(ArmMatrixCalculateTest);
    {
        using TestMatrix = rmdev::ArmMatrix<float, 3, 3>;

        TestMatrix mat1{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
        const TestMatrix mat2{9.0f, 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f};

        TestMatrix mat1_add_mat2;
        rmdev::add(mat1_add_mat2, mat1, mat2);
        const TestMatrix mat1_add_mat2_expected{10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f};
        bool mat1_add_mat2_is_equal_to_expected = true;
        for (std::size_t i = 1; i <= 3; ++i) {
            for (std::size_t j = 1; j <= 3; ++j) {
                if (!rmdev::weakEqu(mat1_add_mat2(i, j), mat1_add_mat2_expected(i, j))) {
                    mat1_add_mat2_is_equal_to_expected = false;
                    break;
                }
            }
            if (!mat1_add_mat2_is_equal_to_expected) {
                break;
            }
        }
        EXPECT_TRUE(mat1_add_mat2_is_equal_to_expected);

        TestMatrix mat1_sub_mat2;
        rmdev::sub(mat1_sub_mat2, mat1, mat2);
        const TestMatrix mat1_sub_mat2_expected{-8.0f, -6.0f, -4.0f, -2.0f, 0.0f, 2.0f, 4.0f, 6.0f, 8.0f};
        bool mat1_sub_mat2_is_equal_to_expected = true;
        for (std::size_t i = 1; i <= 3; ++i) {
            for (std::size_t j = 1; j <= 3; ++j) {
                if (!rmdev::weakEqu(mat1_sub_mat2(i, j), mat1_sub_mat2_expected(i, j))) {
                    mat1_sub_mat2_is_equal_to_expected = false;
                    break;
                }
            }
            if (!mat1_sub_mat2_is_equal_to_expected) {
                break;
            }
        }
        EXPECT_TRUE(mat1_sub_mat2_is_equal_to_expected);

        TestMatrix mat1_mul_5;
        rmdev::mul(mat1_mul_5, mat1, 5.0f);
        const TestMatrix mat1_mul_5_expected{5.0f, 10.0f, 15.0f, 20.0f, 25.0f, 30.0f, 35.0f, 40.0f, 45.0f};
        bool mat1_mul_5_is_equal_to_expected = true;
        for (std::size_t i = 1; i <= 3; ++i) {
            for (std::size_t j = 1; j <= 3; ++j) {
                if (!rmdev::weakEqu(mat1_mul_5(i, j), mat1_mul_5_expected(i, j))) {
                    mat1_mul_5_is_equal_to_expected = false;
                    break;
                }
            }
            if (!mat1_mul_5_is_equal_to_expected) {
                break;
            }
        }
        EXPECT_TRUE(mat1_mul_5_is_equal_to_expected);

        mat1_mul_5.clear();
        rmdev::mul(mat1_mul_5, 5.0f, mat1);
        mat1_mul_5_is_equal_to_expected = true;
        for (std::size_t i = 1; i <= 3; ++i) {
            for (std::size_t j = 1; j <= 3; ++j) {
                if (!rmdev::weakEqu(mat1_mul_5(i, j), mat1_mul_5_expected(i, j))) {
                    mat1_mul_5_is_equal_to_expected = false;
                    break;
                }
            }
            if (!mat1_mul_5_is_equal_to_expected) {
                break;
            }
        }
        EXPECT_TRUE(mat1_mul_5_is_equal_to_expected);

        TestMatrix mat1_mul_mat2;
        rmdev::mul(mat1_mul_mat2, mat1, mat2);
        const TestMatrix mat1_mul_mat2_expected{30.0f, 24.0f, 18.0f, 84.0f, 69.0f, 54.0f, 138.0f, 114.0f, 90.0f};
        bool mat1_mul_mat2_is_equal_to_expected = true;
        for (std::size_t i = 1; i <= 3; ++i) {
            for (std::size_t j = 1; j <= 3; ++j) {
                if (!rmdev::weakEqu(mat1_mul_mat2(i, j), mat1_mul_mat2_expected(i, j))) {
                    mat1_mul_mat2_is_equal_to_expected = false;
                    break;
                }
            }
            if (!mat1_mul_mat2_is_equal_to_expected) {
                break;
            }
        }
        EXPECT_TRUE(mat1_mul_mat2_is_equal_to_expected);

        const rmdev::ArmMatrix<float, 4, 2> mat3{{1.0f, 2.0f}, {3.0f, 4.0f}, {5.0f, 6.0f}, {7.0f, 8.0f}};
        rmdev::ArmMatrix<float, 2, 4> mat3_trans;
        rmdev::trans(mat3_trans, mat3);
        const decltype(mat3_trans) mat3_trans_expected{{1.0f, 3.0f, 5.0f, 7.0f}, {2.0f, 4.0f, 6.0f, 8.0f}};
        EXPECT_TRUE(mat3_trans == mat3_trans_expected);

        TestMatrix mat1_inv{mat1};
        EXPECT_TRUE(mat1_inv == mat1);
        auto p_result = rmdev::inv(mat1_inv, mat1);
        EXPECT_TRUE(p_result == nullptr);

        const TestMatrix mat5_origin{{1, 2, 3}, {4, 7, 6}, {7, 8, 10}};
        TestMatrix mat5{mat5_origin};
        TestMatrix mat5_inv;
        p_result = rmdev::inv(mat5_inv, mat5);
        EXPECT_TRUE(p_result != nullptr);
        EXPECT_TRUE(mat5_inv != mat5);
        const TestMatrix mat5_inv_expected{{-0.88f, -0.16f, 0.36f}, {-0.08f, 0.44f, -0.24f}, {0.68f, -0.24f, 0.04f}};
        EXPECT_TRUE(mat5_inv == mat5_inv_expected);

        mat5 = mat5_origin;
        TestMatrix mat5_div_0;
        p_result = rmdev::div(mat5_div_0, mat5, 0.0f);
        EXPECT_TRUE(p_result == nullptr);

        mat5 = mat5_origin;
        TestMatrix mat5_div_2;
        p_result = rmdev::div(mat5_div_2, mat5, 2.0f);
        const TestMatrix mat5_div2_expected{{0.5f, 1.0f, 1.5f}, {2.0f, 3.5f, 3.0f}, {3.5f, 4.0f, 5.0f}};
        EXPECT_TRUE(p_result != nullptr);
        EXPECT_TRUE(mat5_div_2.equ(mat5_div2_expected, 0.01f));

        mat5 = mat5_origin;
        p_result = rmdev::div(mat5_div_2, 2.0f, mat5);
        TestMatrix mat5_inv_expected_mul2;
        rmdev::mul(mat5_inv_expected_mul2, mat5_inv_expected, 2.0f);
        const TestMatrix scale2_div_5_expected{mat5_inv_expected_mul2};
        EXPECT_TRUE(p_result != nullptr);
        EXPECT_TRUE(mat5_div_2.equ(scale2_div_5_expected));
    }
    TEST_CASE_END();
}

void matrixTest()
{
    RUN_SUIT(ArmMatCInterfaceTest);
    RUN_SUIT(ArmMatrixBasicTest);
    RUN_SUIT(ArmMatrixSpecialConstructTest);
    RUN_SUIT(ArmMatrixCalcTest);
}
