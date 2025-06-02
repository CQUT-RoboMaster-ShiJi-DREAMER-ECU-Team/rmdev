/**
 * @file Matrix_impl.cppm
 * @module rmdev.Matrix:impl
 * @author 杜以成
 * @date 2025-06-02
 * @brief 矩阵计算实现
 */

module;

#include <cstddef>
#include <cstring>
#include "etl/type_traits.h"
#include "arm_math.h"

export module rmdev.Matrix:impl;
import :def;

export namespace rmdev {

template<std::size_t row, std::size_t col>
Matrix<float, row, col>::Matrix()
{
    arm_mat_init_f32(&matrix, row, col, data);
}

template<std::size_t row, std::size_t col>
Matrix<float, row, col>::Matrix(const Matrix& other)
{
    std::memcpy(this->data, other.data, sizeof this->data);
    this->matrix = {.numCols = other.matrix.numCols, .numRows = other.matrix.numRows, .pData = this->data};
}

template<std::size_t row, std::size_t col>
Matrix<float, row, col>& Matrix<float, row, col>::operator=(const Matrix& other)
{
    if (this != &other) {
        std::memcpy(data, other.data, sizeof data);
    }
    return *this;
}

template<std::size_t row, std::size_t col>
Matrix<float, row, col>& Matrix<float, row, col>::add(Matrix& result, const Matrix& a, const Matrix& b)
{
    arm_mat_add_f32(&a.matrix, &b.matrix, &result.matrix);

    return result;
}

template<std::size_t row, std::size_t col>
Matrix<float, row, col>& Matrix<float, row, col>::subtract(Matrix& result, const Matrix& a, const Matrix& b)
{
    arm_mat_sub_f32(&a.matrix, &b.matrix, &result.matrix);

    return result;
}

template<std::size_t row, std::size_t col>
template<std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
auto Matrix<float, row, col>::multiply(Matrix<float, rowa, colb>& result,
                                       const Matrix<float, rowa, cola>& a,
                                       const Matrix<float, rowb, colb>& b)
    -> etl::enable_if_t<cola == rowb, Matrix<float, rowa, colb>&>
{
    arm_mat_mult_f32(&a.matrix, &b.matrix, &result.matrix);

    return result;
}

template<std::size_t row, std::size_t col>
Matrix<float, row, col>& Matrix<float, row, col>::multiply(Matrix& result, const Matrix& a, const float scalar)
{
    arm_mat_scale_f32(&a.matrix, scalar, &result.matrix);

    return result;
}

template<std::size_t row, std::size_t col>
Matrix<float, row, col>& Matrix<float, row, col>::multiply(Matrix& result, const float scalar, const Matrix& a)
{
    return multiply(result, a, scalar);
}

template<std::size_t row, std::size_t col>
Matrix<float, col, row>& Matrix<float, row, col>::transpose(Matrix<float, col, row>& result,
                                                            const Matrix<float, row, col>& a)
{
    arm_mat_trans_f32(&a.matrix, &result.matrix);

    return result;
}

template<std::size_t row, std::size_t col>
auto Matrix<float, row, col>::inverse(Matrix& result, const Matrix& a) -> etl::enable_if_t<row == col, Matrix&>
{
    arm_mat_inverse_f32(&a.matrix, &result.matrix);

    return result;
}

template<std::size_t row, std::size_t col>
auto Matrix<float, row, col>::determinant(Matrix& a) -> etl::enable_if_t<row == col, float>
{
    float det;

    return det;
}

}  // namespace rmdev
