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
#include "etl/algorithm.h"

#include "arm_math.h"

#include "rmdev/concepts.hpp"

export module rmdev.Matrix:CMSIS_DSP;

// ================ declares ================
export namespace rmdev {

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
class Matrix;

template<std::size_t row, std::size_t col>
class Matrix<float, row, col>
{
public:
    static_assert(etl::is_same_v<float, float32_t>);

    using Type = float;
    using ArmMatrixType = arm_matrix_instance_f32;

    // 由于 CMSIS-DSP 的矩阵加法函数需要分配一个新的矩阵，常规运算符难以实现，因此删除基本运算符
    Matrix& operator+(const Matrix& other) = delete;
    Matrix& operator-(const Matrix& other) = delete;
    Matrix& operator*(const Matrix& other) = delete;
    Matrix& operator/(const Matrix& other) = delete;

    Matrix&& trans() = delete;

    /**
     *
     */
    Matrix();

    Matrix(const Matrix& other);

    explicit Matrix(const Type mat_data[row * col]);

    explicit Matrix(const Type mat_data[row][col]);

    Matrix(std::initializer_list<Type> mat_data);

    Matrix(std::initializer_list<std::initializer_list<Type>> mat_data);

    Matrix& operator=(const Matrix& other);

    bool operator==(const Matrix& other) const;

    bool operator!=(const Matrix& other) const;

    template<std::size_t row_, std::size_t col_>
    friend Matrix<Type, row_, col_>& add(Matrix<Type, row_, col_>& result,
                                         const Matrix<Type, row_, col_>& a,
                                         const Matrix<Type, row_, col_>& b);

    template<std::size_t row_, std::size_t col_>
    friend Matrix<Type, row_, col_>& sub(Matrix<Type, row_, col_>& result,
                                         const Matrix<Type, row_, col_>& a,
                                         const Matrix<Type, row_, col_>& b);

    template<std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
        requires MatrixCouldMultiplied<rowa, cola, rowb, colb>
    friend Matrix<Type, rowa, colb>& mul(Matrix<Type, rowa, colb>& result,
                                         const Matrix<Type, rowa, cola>& a,
                                         const Matrix<Type, rowb, colb>& b);

    static Matrix& multiply(Matrix& result, const Matrix& a, Type scalar);

    static Matrix& multiply(Matrix& result, Type scalar, const Matrix& a);

    static Matrix<Type, col, row>& transpose(Matrix<Type, col, row>& result, const Matrix<Type, row, col>& a);

    template<std::size_t row_, std::size_t col_>
    friend auto inverse(Matrix<Type, row_, col_>& result, const Matrix<Type, row_, col_>& a)
        -> etl::enable_if_t<row_ == col_, Matrix<Type, row_, col_>&>;

    static Type determinant(Matrix& a);

    Type* at(std::size_t r, std::size_t c);

    Type& operator()(std::size_t r, std::size_t c);

private:
    ArmMatrixType matrix{};
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
    this->matrix = {.numRows = other.matrix.numRows, .numCols = other.matrix.numCols, .pData = this->data};

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
Matrix<float, row, col>::Matrix(const std::initializer_list<float> mat_data)
{
    std::memcpy(data, mat_data.begin(), sizeof data);
    arm_mat_init_f32(&matrix, row, col, data);
}

template<std::size_t row, std::size_t col>
Matrix<float, row, col>::Matrix(const std::initializer_list<std::initializer_list<float>> mat_data)
{
    std::size_t i = 0;
    for (const auto& row_data : mat_data) {
        std::memcpy(&data[i * col], row_data.begin(), sizeof(float) * row_data.size());
        ++i;
    }
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
bool Matrix<float, row, col>::operator==(const Matrix& other) const
{
    return etl::equal(data, data + row * col, other.data);
}

template<std::size_t row, std::size_t col>
bool Matrix<float, row, col>::operator!=(const Matrix& other) const
{
    return !(this->operator==(other));
}

template<std::size_t row, std::size_t col>
Matrix<float, row, col>& add(Matrix<float, row, col>& result,
                             const Matrix<float, row, col>& a,
                             const Matrix<float, row, col>& b)
{
    arm_mat_add_f32(&a.matrix, &b.matrix, &result.matrix);

    return result;
}

template<std::size_t row, std::size_t col>
Matrix<float, row, col>& sub(Matrix<float, row, col>& result,
                             const Matrix<float, row, col>& a,
                             const Matrix<float, row, col>& b)
{
    arm_mat_sub_f32(&a.matrix, &b.matrix, &result.matrix);

    return result;
}

template<std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
    requires MatrixCouldMultiplied<rowa, cola, rowb, colb>
Matrix<float, rowa, colb>& mul(Matrix<float, rowa, colb>& result,
                               const Matrix<float, rowa, cola>& a,
                               const Matrix<float, rowb, colb>& b)
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
