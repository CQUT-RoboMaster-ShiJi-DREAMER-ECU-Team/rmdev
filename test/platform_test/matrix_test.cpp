/**
 * @file matrix_test.cpp
 * @author 杜以成
 * @date 2025-06-04
 * @brief 矩阵运算测试
 */

#include "rmdev_test_framework.h"

#include "etl/array.h"

import rmdev.Matrix;
import rmdev.util.math;

static void matrixBasicTest()
{
    using rmdev::floatEqu;

    RMDEV_TEST_ITEM("Matrix basic test");

    rmdev::Matrix<float, 3, 3> mat1;

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

    rmdev::Matrix<float, 3, 3> mat2{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
    rmdev::Matrix<float, 3, 3> mat3{{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}, {7.0f, 8.0f, 9.0f}};
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

    rmdev::Matrix<float, 3, 3> mat4{mat1};
    RMDEV_TEST_ASSERT(mat4 == mat1);

    rmdev::Matrix<float, 3, 3> mat4_2(mat1);
    RMDEV_TEST_ASSERT(mat4_2 == mat1);

    const float data_equ_to_mat2[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
    rmdev::Matrix<float, 3, 3> mat5{data_equ_to_mat2};
    rmdev::Matrix<float, 3, 3> mat5_2(data_equ_to_mat2);
    RMDEV_TEST_ASSERT(mat5 == mat2);
    RMDEV_TEST_ASSERT(mat5_2 == mat2);

    const float data_equ_to_mat3[3][3] = {{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}, {7.0f, 8.0f, 9.0f}};
    rmdev::Matrix<float, 3, 3> mat6{data_equ_to_mat3};
    rmdev::Matrix<float, 3, 3> mat6_2(data_equ_to_mat3);
    RMDEV_TEST_ASSERT(mat6 == mat3);
    RMDEV_TEST_ASSERT(mat6_2 == mat3);
    RMDEV_TEST_ASSERT(mat5 == mat6);

    rmdev::Matrix<float, 3, 3> mat7 = mat2;
    RMDEV_TEST_ASSERT(mat7 == mat2);
}

static void matrixCalcTest()
{
    using TestMatrix = rmdev::Matrix<float, 3, 3>;

    TestMatrix mat1{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
    TestMatrix mat2{9.0f, 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f};

    TestMatrix mat1_add_mat2;
    rmdev::Matrix::add(mat1_add_mat2, mat1, mat2);
    TestMatrix mat1_add_mat2_expected{10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f};
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
}

void matrixTest()
{
    matrixBasicTest();
    matrixCalcTest();
}
