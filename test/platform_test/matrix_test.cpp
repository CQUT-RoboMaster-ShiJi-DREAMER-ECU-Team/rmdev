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

void matrix_test()
{
    using rmdev::floatEqu;

    rmdev::Matrix<float, 3, 3> mat1;

    RMDEV_TEST_ASSERT("get Matrix item", floatEqu(mat1(0, 0), 0.0f));

    mat1(1, 1) = 1.0f;
    mat1(2, 1) = 2.0f;
    mat1(3, 3) = 3.0f;
    RMDEV_TEST_ASSERT("get Matrix item after set",
                      floatEqu(mat1(1, 1), 1.0f) && floatEqu(mat1(2, 1), 2.0f) && floatEqu(mat1(3, 3), 3.0f));

    *mat1.at(3, 1) = 245.0f;
    *mat1.at(2, 3) = 123.0f;
    *mat1.at(1, 2) = 456.0f;
    RMDEV_TEST_ASSERT(
        "get Matrix item after set by at()",
        floatEqu(*mat1.at(3, 1), 245.0f) && floatEqu(*mat1.at(2, 3), 123.0f) && floatEqu(*mat1.at(1, 2), 456.0f));

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
    RMDEV_TEST_ASSERT("at() return nullptr for out of range", nullnum_in_mat1 == nullnum_in_mat1_expected);
}
