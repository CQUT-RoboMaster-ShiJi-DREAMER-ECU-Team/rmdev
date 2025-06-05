/**
 * @file cmsis_dsp_mat_wrapper.cppm
 * @module rmdev.Matrix:CMSIS_DSP
 * @author 杜以成
 * @date 2025-06-03
 * @brief CMSIS-DSP 矩阵运算封装
 */

#if RMDEV_USE_CMSIS_DSP

module;

#include <cstddef>
#include <cstring>

#include "etl/type_traits.h"
#include "etl/initializer_list.h"

#include "arm_math.h"

export module rmdev.Matrix:CMSIS_DSP;

import rmdev.error_handler;

// ================ declares ================
export namespace rmdev {

template<typename Type, std::size_t row, std::size_t col>
class Matrix;

template<std::size_t row, std::size_t col>
class Matrix<float, row, col>
{
public:
    static_assert(etl::is_same_v<float, float32_t>);

    /**
     *
     */
    Matrix();

    Matrix(const Matrix& other);

    explicit Matrix(const float mat_data[row * col]);

    explicit Matrix(const float mat_data[row][col]);

    Matrix(const std::initializer_list<float> mat_data)
    {
        std::memcpy(data, mat_data.begin(), sizeof data);
        arm_mat_init_f32(&matrix, row, col, data);
    }

    Matrix(const std::initializer_list<std::initializer_list<float>> mat_data)
    {
        std::size_t i = 0;
        for (const auto& row_data : mat_data) {
            std::memcpy(&data[i * col], row_data.begin(), sizeof(float) * row_data.size());
            ++i;
        }
        arm_mat_init_f32(&matrix, row, col, data);
    }

    Matrix& operator=(const Matrix& other);

    static Matrix& add(Matrix& result, const Matrix& a, const Matrix& b);

    static Matrix& subtract(Matrix& result, const Matrix& a, const Matrix& b);

    template<std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
    friend auto multiply(Matrix<float, rowa, colb>& result,
                         const Matrix<float, rowa, cola>& a,
                         const Matrix<float, rowb, colb>& b)
        -> etl::enable_if_t<cola == rowb, Matrix<float, rowa, colb>&>;

    static Matrix& multiply(Matrix& result, const Matrix& a, float scalar);

    static Matrix& multiply(Matrix& result, float scalar, const Matrix& a);

    static Matrix<float, col, row>& transpose(Matrix<float, col, row>& result, const Matrix<float, row, col>& a);

    template<std::size_t row_, std::size_t col_>
    friend auto inverse(Matrix<float, row_, col_>& result, const Matrix<float, row_, col_>& a)
        -> etl::enable_if_t<row_ == col_, Matrix<float, row_, col_>&>;

    static float determinant(Matrix& a);

    float* at(std::size_t r, std::size_t c);

    float& operator()(std::size_t r, std::size_t c);

private:
    arm_matrix_instance_f32 matrix{};
    float32_t data[row * col]{};
};

template<std::size_t row, std::size_t col>
class Matrix<double, row, col>
{
public:
    static_assert(etl::is_same_v<double, float64_t>);
    /**
     *
     */
    Matrix()
    {
        arm_mat_init_f64(&matrix, row, col, data);
    }

private:
    arm_matrix_instance_f64 matrix{};
    float64_t data[row * col]{};
};

}  // namespace rmdev

// ================== implements ==================
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

    arm_mat_init_f32(&matrix, row, col, data);
}

template<std::size_t row, std::size_t col>
Matrix<float, row, col>::Matrix(const float mat_data[row * col])
{
    std::memcpy(data, mat_data, sizeof data);
    arm_mat_init_f32(&matrix, row, col, data);
}

template<std::size_t row, std::size_t col>
Matrix<float, row, col>::Matrix(const float mat_data[row][col])
{
    std::memcpy(data, mat_data, sizeof data);
    arm_mat_init_f32(&matrix, row, col, data);
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

template<std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
auto multiply(Matrix<float, rowa, colb>& result, const Matrix<float, rowa, cola>& a, const Matrix<float, rowb, colb>& b)
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
auto inverse(Matrix<float, row, col>& result, const Matrix<float, row, col>& a)
    -> etl::enable_if_t<row == col, Matrix<float, row, col>&>
{
    arm_mat_inverse_f32(&a.matrix, &result.matrix);

    return result;
}

template<std::size_t row, std::size_t col>
float Matrix<float, row, col>::determinant(Matrix& a)
{
    if constexpr (row != col) {
        return 0.0f;
    }

    float det;

    return det;
}

template<std::size_t row, std::size_t col>
float* Matrix<float, row, col>::at(const std::size_t r, const std::size_t c)
{
    if (r < 1U || c < 1U) {
        return nullptr;
    }
    if ((r - 1U) >= row || (c - 1U) >= col) {
        return nullptr;
    }

    return &data[(r - 1U) * col + (c - 1U)];
}

template<std::size_t row, std::size_t col>
float& Matrix<float, row, col>::operator()(const std::size_t r, const std::size_t c)
{
    return data[(r - 1U) * col + (c - 1U)];
}

}  // namespace rmdev

#endif  // RMDEV_USE_CMSIS_DSP
