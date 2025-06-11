/**
 * @file matrix_test.cpp
 * @author 杜以成
 * @date 2025-06-04
 * @brief 矩阵运算测试
 */

#include "rmdev_test_framework.h"

#include "etl/array.h"

import rmdev.util.Matrix;
import rmdev.util.math;

static void armMatrixBasicTest()
{
    using rmdev::floatEqu;

    RMDEV_TEST_ITEM("ArmMatrix Basic Test");

    rmdev::ArmMatrix<float, 3, 3> mat1;

    RMDEV_TEST_ASSERT(floatEqu(mat1(1, 1), 0.0f));

    mat1(1, 1) = 1.0f;
    mat1(2, 1) = 2.0f;
    mat1(3, 3) = 3.0f;
    RMDEV_TEST_ASSERT(floatEqu(mat1(1, 1), 1.0f) && floatEqu(mat1(2, 1), 2.0f) && floatEqu(mat1(3, 3), 3.0f));

    *mat1.at(3, 1) = 245.0f;
    *mat1.at(2, 3) = 123.0f;
    *mat1.at(1, 2) = 456.0f;
    RMDEV_TEST_ASSERT(floatEqu(*mat1.at(3, 1), 245.0f) && floatEqu(*mat1.at(2, 3), 123.0f) &&
                      floatEqu(*mat1.at(1, 2), 456.0f));

    etl::array nullnum_in_mat1{mat1.at(0, 0),
                               mat1.at(0, 2),
                               mat1.at(3, 4),
                               mat1.at(-1, 2),
                               mat1.at(4, 3),
                               mat1.at(4, 4),
                               mat1.at(114514, 1919810)};
    using nullnum_arr_in_mat1_t = decltype(nullnum_in_mat1);
    nullnum_arr_in_mat1_t nullnum_in_mat1_expected{};
    nullnum_in_mat1_expected.fill(nullptr);
    RMDEV_TEST_ASSERT(nullnum_in_mat1 == nullnum_in_mat1_expected);

    rmdev::ArmMatrix<float, 3, 3> mat2{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
    rmdev::ArmMatrix<float, 3, 3> mat3{{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}, {7.0f, 8.0f, 9.0f}};
    RMDEV_TEST_ASSERT(
        rmdev::floatEqu(mat2(1, 1), 1.0f) && rmdev::floatEqu(mat2(1, 2), 2.0f) && rmdev::floatEqu(mat2(1, 3), 3.0f) &&
        rmdev::floatEqu(mat2(2, 1), 4.0f) && rmdev::floatEqu(mat2(2, 2), 5.0f) && rmdev::floatEqu(mat2(2, 3), 6.0f) &&
        rmdev::floatEqu(mat2(3, 1), 7.0f) && rmdev::floatEqu(mat2(3, 2), 8.0f) && rmdev::floatEqu(mat2(3, 3), 9.0f));

    bool mat2_and_mat3_are_equal = true;
    for (std::size_t i = 1; i <= 3; ++i) {
        for (std::size_t j = 1; j <= 3; ++j) {
            if (!rmdev::floatEqu(mat2(i, j), mat3(i, j))) {
                mat2_and_mat3_are_equal = false;
                break;
            }
        }
        if (!mat2_and_mat3_are_equal) {
            break;
        }
    }
    RMDEV_TEST_ASSERT(mat2_and_mat3_are_equal);
    RMDEV_TEST_ASSERT(mat2 == mat3);
    RMDEV_TEST_ASSERT(mat3 == mat2);
    RMDEV_TEST_ASSERT(!(mat2 == mat1));
    RMDEV_TEST_ASSERT(!(mat2 != mat3));
    RMDEV_TEST_ASSERT(mat2 != mat1);

    mat1 = mat2;
    RMDEV_TEST_ASSERT(mat1 == mat2);

    rmdev::ArmMatrix<float, 3, 3> mat4{mat1};
    RMDEV_TEST_ASSERT(mat4 == mat1);

    rmdev::ArmMatrix<float, 3, 3> mat4_2(mat1);
    RMDEV_TEST_ASSERT(mat4_2 == mat1);

    const float data_equ_to_mat2[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
    rmdev::ArmMatrix<float, 3, 3> mat5{data_equ_to_mat2};
    rmdev::ArmMatrix<float, 3, 3> mat5_2(data_equ_to_mat2);
    RMDEV_TEST_ASSERT(mat5 == mat2);
    RMDEV_TEST_ASSERT(mat5_2 == mat2);

    const float data_equ_to_mat3[3][3] = {{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}, {7.0f, 8.0f, 9.0f}};
    rmdev::ArmMatrix<float, 3, 3> mat6{data_equ_to_mat3};
    rmdev::ArmMatrix<float, 3, 3> mat6_2(data_equ_to_mat3);
    RMDEV_TEST_ASSERT(mat6 == mat3);
    RMDEV_TEST_ASSERT(mat6_2 == mat3);
    RMDEV_TEST_ASSERT(mat5 == mat6);

    rmdev::ArmMatrix<float, 3, 3> mat7 = mat2;
    RMDEV_TEST_ASSERT(mat7 == mat2);

    const rmdev::ArmMatrix<float, 3, 3> mat8{mat2};
    RMDEV_TEST_ASSERT(mat8 == mat2);

    bool mat2_and_mat8_are_equal = true;
    for (std::size_t i = 1; i <= 3; ++i) {
        for (std::size_t j = 1; j <= 3; ++j) {
            if (!floatEqu(mat2(i, j), mat8(i, j))) {
                mat2_and_mat8_are_equal = false;
                break;
            }
        }
        if (!mat2_and_mat8_are_equal) {
            break;
        }
    }
    RMDEV_TEST_ASSERT(mat2_and_mat8_are_equal);

    bool mat2_and_mat8_are_equal_use_at = true;
    for (std::size_t i = 1; i <= 3; ++i) {
        for (std::size_t j = 1; j <= 3; ++j) {
            if (!floatEqu(*mat8.at(i, j), mat2(i, j))) {
                mat2_and_mat8_are_equal_use_at = false;
                break;
            }
        }
        if (!mat2_and_mat8_are_equal_use_at) {
            break;
        }
    }
    RMDEV_TEST_ASSERT(mat2_and_mat8_are_equal_use_at);
}

static void armMatrixCalcTest()
{
    RMDEV_TEST_ITEM("ArmMatrix Calculate Test");

    using TestMatrix = rmdev::ArmMatrix<float, 3, 3>;

    const TestMatrix mat1{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
    const TestMatrix mat2{9.0f, 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f};

    TestMatrix mat1_add_mat2;
    rmdev::add(mat1_add_mat2, mat1, mat2);
    const TestMatrix mat1_add_mat2_expected{10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f};
    bool mat1_add_mat2_is_equal_to_expected = true;
    for (std::size_t i = 1; i <= 3; ++i) {
        for (std::size_t j = 1; j <= 3; ++j) {
            if (!rmdev::floatEqu(mat1_add_mat2(i, j), mat1_add_mat2_expected(i, j))) {
                mat1_add_mat2_is_equal_to_expected = false;
                break;
            }
        }
        if (!mat1_add_mat2_is_equal_to_expected) {
            break;
        }
    }
    RMDEV_TEST_ASSERT(mat1_add_mat2_is_equal_to_expected);

    TestMatrix mat1_sub_mat2;
    rmdev::sub(mat1_sub_mat2, mat1, mat2);
    const TestMatrix mat1_sub_mat2_expected{-8.0f, -6.0f, -4.0f, -2.0f, 0.0f, 2.0f, 4.0f, 6.0f, 8.0f};
    bool mat1_sub_mat2_is_equal_to_expected = true;
    for (std::size_t i = 1; i <= 3; ++i) {
        for (std::size_t j = 1; j <= 3; ++j) {
            if (!rmdev::floatEqu(mat1_sub_mat2(i, j), mat1_sub_mat2_expected(i, j))) {
                mat1_sub_mat2_is_equal_to_expected = false;
                break;
            }
        }
        if (!mat1_sub_mat2_is_equal_to_expected) {
            break;
        }
    }
    RMDEV_TEST_ASSERT(mat1_sub_mat2_is_equal_to_expected);

    TestMatrix mat1_mul_5;
    rmdev::mul(mat1_mul_5, mat1, 5.0f);
    const TestMatrix mat1_mul_5_expected{5.0f, 10.0f, 15.0f, 20.0f, 25.0f, 30.0f, 35.0f, 40.0f, 45.0f};
    bool mat1_mul_5_is_equal_to_expected = true;
    for (std::size_t i = 1; i <= 3; ++i) {
        for (std::size_t j = 1; j <= 3; ++j) {
            if (!rmdev::floatEqu(mat1_mul_5(i, j), mat1_mul_5_expected(i, j))) {
                mat1_mul_5_is_equal_to_expected = false;
                break;
            }
        }
        if (!mat1_mul_5_is_equal_to_expected) {
            break;
        }
    }
    RMDEV_TEST_ASSERT(mat1_mul_5_is_equal_to_expected);

    mat1_mul_5 = {};
    rmdev::mul(mat1_mul_5, 5.0f, mat1);
    mat1_mul_5_is_equal_to_expected = true;
    for (std::size_t i = 1; i <= 3; ++i) {
        for (std::size_t j = 1; j <= 3; ++j) {
            if (!rmdev::floatEqu(mat1_mul_5(i, j), mat1_mul_5_expected(i, j))) {
                mat1_mul_5_is_equal_to_expected = false;
                break;
            }
        }
        if (!mat1_mul_5_is_equal_to_expected) {
            break;
        }
    }
    RMDEV_TEST_ASSERT(mat1_mul_5_is_equal_to_expected);

    TestMatrix mat1_mul_mat2;
    rmdev::mul(mat1_mul_mat2, mat1, mat2);
    const TestMatrix mat1_mul_mat2_expected{30.0f, 24.0f, 18.0f, 84.0f, 69.0f, 54.0f, 138.0f, 114.0f, 90.0f};
    bool mat1_mul_mat2_is_equal_to_expected = true;
    for (std::size_t i = 1; i <= 3; ++i) {
        for (std::size_t j = 1; j <= 3; ++j) {
            if (!rmdev::floatEqu(mat1_mul_mat2(i, j), mat1_mul_mat2_expected(i, j))) {
                mat1_mul_mat2_is_equal_to_expected = false;
                break;
            }
        }
        if (!mat1_mul_mat2_is_equal_to_expected) {
            break;
        }
    }
    RMDEV_TEST_ASSERT(mat1_mul_mat2_is_equal_to_expected);

    const rmdev::ArmMatrix<float, 4, 2> mat3{{1.0f, 2.0f}, {3.0f, 4.0f}, {5.0f, 6.0f}, {7.0f, 8.0f}};
    rmdev::ArmMatrix<float, 2, 4> mat3_trans;
    rmdev::trans(mat3_trans, mat3);
    const decltype(mat3_trans) mat3_trans_expected{{1.0f, 3.0f, 5.0f, 7.0f}, {2.0f, 4.0f, 6.0f, 8.0f}};
    bool mat3_trans_is_equal_to_expected = true;
    for (std::size_t i = 1; i <= 3; ++i) {
        for (std::size_t j = 1; j <= 3; ++j) {
            if (!rmdev::floatEqu(mat3_trans(i, j), mat3_trans_expected(i, j))) {
                mat3_trans_is_equal_to_expected = false;
                break;
            }
        }
        if (!mat3_trans_is_equal_to_expected) {
            break;
        }
    }
    RMDEV_TEST_ASSERT(mat3_trans_is_equal_to_expected);

    TestMatrix mat1_inv{mat1};
    RMDEV_TEST_ASSERT(mat1_inv == mat1);
    auto p_result = rmdev::inv(mat1_inv, mat1);
    RMDEV_TEST_ASSERT(p_result == nullptr);
    // RMDEV_TEST_ASSERT(mat1_inv == mat1);  // 虽然求逆矩阵失败，但计算结果仍然会发生改变

    TestMatrix mat5{{1, 2, 3}, {4, 7, 6}, {7, 8, 10}};
    TestMatrix mat5_inv;
    p_result = rmdev::inv(mat5_inv, mat5);
    RMDEV_TEST_ASSERT(p_result != nullptr);
    RMDEV_TEST_ASSERT(mat5_inv != mat5);
    const TestMatrix mat5_inv_expected{{-0.88f, -0.16f, 0.36f}, {-0.08f, 0.44f, -0.24f}, {0.68f, -0.24f, 0.04f}};
    RMDEV_TEST_ASSERT(mat5_inv == mat5_inv_expected);
}

void matrixTest()
{
    armMatrixBasicTest();
    armMatrixCalcTest();
}
