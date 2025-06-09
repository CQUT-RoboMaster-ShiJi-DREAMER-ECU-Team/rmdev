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
class ArmMatrix;

template<std::size_t row, std::size_t col>
class ArmMatrix<float, row, col>
{
public:
    static_assert(etl::is_same_v<float, float32_t>);

    using Type = float;
    using ArmMatrixType = arm_matrix_instance_f32;

    // 由于 CMSIS-DSP 的矩阵加法函数需要分配一个新的矩阵，常规运算符难以实现，因此删除基本运算符
    ArmMatrix& operator+(const ArmMatrix& other) = delete;
    ArmMatrix& operator-(const ArmMatrix& other) = delete;
    ArmMatrix& operator*(const ArmMatrix& other) = delete;
    ArmMatrix& operator/(const ArmMatrix& other) = delete;

    ArmMatrix&& trans() = delete;

    /**
     *
     */
    ArmMatrix();

    ArmMatrix(const ArmMatrix& other);

    explicit ArmMatrix(const Type mat_data[row * col]);

    explicit ArmMatrix(const Type mat_data[row][col]);

    ArmMatrix(std::initializer_list<Type> mat_data);

    ArmMatrix(std::initializer_list<std::initializer_list<Type>> mat_data);

    ArmMatrix& operator=(const ArmMatrix& other);

    constexpr bool operator==(const ArmMatrix& other) const;

    constexpr bool operator!=(const ArmMatrix& other) const;

    template<std::size_t row_, std::size_t col_>
    friend ArmMatrix<Type, row_, col_>& add(ArmMatrix<Type, row_, col_>& result,
                                            const ArmMatrix<Type, row_, col_>& a,
                                            const ArmMatrix<Type, row_, col_>& b);

    template<std::size_t row_, std::size_t col_>
    friend ArmMatrix<Type, row_, col_>& sub(ArmMatrix<Type, row_, col_>& result,
                                            const ArmMatrix<Type, row_, col_>& a,
                                            const ArmMatrix<Type, row_, col_>& b);

    template<std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
        requires MatrixCouldMultiplied<rowa, cola, rowb, colb>
    friend ArmMatrix<Type, rowa, colb>& mul(ArmMatrix<Type, rowa, colb>& result,
                                            const ArmMatrix<Type, rowa, cola>& a,
                                            const ArmMatrix<Type, rowb, colb>& b);

    static ArmMatrix& multiply(ArmMatrix& result, const ArmMatrix& a, Type scalar);

    static ArmMatrix& multiply(ArmMatrix& result, Type scalar, const ArmMatrix& a);

    static ArmMatrix<Type, col, row>& transpose(ArmMatrix<Type, col, row>& result, const ArmMatrix<Type, row, col>& a);

    template<std::size_t row_, std::size_t col_>
    friend auto inverse(ArmMatrix<Type, row_, col_>& result, const ArmMatrix<Type, row_, col_>& a)
        -> etl::enable_if_t<row_ == col_, ArmMatrix<Type, row_, col_>&>;

    static Type determinant(ArmMatrix& a);

    Type* at(std::size_t r, std::size_t c);

    Type& operator()(std::size_t r, std::size_t c);

private:
    ArmMatrixType matrix{};
    float32_t data[row * col]{};
};

template<std::size_t row, std::size_t col>
class ArmMatrix<double, row, col>;  // todo 待完成其他类型的特化

}  // namespace rmdev

// ================== implements ==================
export namespace rmdev {

template<std::size_t row, std::size_t col>
ArmMatrix<float, row, col>::ArmMatrix()
{
    arm_mat_init_f32(&matrix, row, col, data);
}

template<std::size_t row, std::size_t col>
ArmMatrix<float, row, col>::ArmMatrix(const ArmMatrix& other)
{
    std::memcpy(this->data, other.data, sizeof this->data);
    this->matrix = {.numRows = other.matrix.numRows, .numCols = other.matrix.numCols, .pData = this->data};

    arm_mat_init_f32(&matrix, row, col, data);
}

template<std::size_t row, std::size_t col>
ArmMatrix<float, row, col>::ArmMatrix(const float mat_data[row * col])
{
    std::memcpy(data, mat_data, sizeof data);
    arm_mat_init_f32(&matrix, row, col, data);
}

template<std::size_t row, std::size_t col>
ArmMatrix<float, row, col>::ArmMatrix(const float mat_data[row][col])
{
    std::memcpy(data, mat_data, sizeof data);
    arm_mat_init_f32(&matrix, row, col, data);
}

template<std::size_t row, std::size_t col>
ArmMatrix<float, row, col>::ArmMatrix(const std::initializer_list<float> mat_data)
{
    std::memcpy(data, mat_data.begin(), sizeof data);
    arm_mat_init_f32(&matrix, row, col, data);
}

template<std::size_t row, std::size_t col>
ArmMatrix<float, row, col>::ArmMatrix(const std::initializer_list<std::initializer_list<float>> mat_data)
{
    std::size_t i = 0;
    for (const auto& row_data : mat_data) {
        std::memcpy(&data[i * col], row_data.begin(), sizeof(float) * row_data.size());
        ++i;
    }
    arm_mat_init_f32(&matrix, row, col, data);
}

template<std::size_t row, std::size_t col>
ArmMatrix<float, row, col>& ArmMatrix<float, row, col>::operator=(const ArmMatrix& other)
{
    if (this != &other) {
        std::memcpy(data, other.data, sizeof data);
    }
    return *this;
}

template<std::size_t row, std::size_t col>
constexpr bool ArmMatrix<float, row, col>::operator==(const ArmMatrix& other) const
{
    return etl::equal(data, data + row * col, other.data);
}

template<std::size_t row, std::size_t col>
constexpr bool ArmMatrix<float, row, col>::operator!=(const ArmMatrix& other) const
{
    return !(this->operator==(other));
}

template<std::size_t row, std::size_t col>
ArmMatrix<float, row, col>& add(ArmMatrix<float, row, col>& result,
                                const ArmMatrix<float, row, col>& a,
                                const ArmMatrix<float, row, col>& b)
{
    arm_mat_add_f32(&a.matrix, &b.matrix, &result.matrix);

    return result;
}

template<std::size_t row, std::size_t col>
ArmMatrix<float, row, col>& sub(ArmMatrix<float, row, col>& result,
                                const ArmMatrix<float, row, col>& a,
                                const ArmMatrix<float, row, col>& b)
{
    arm_mat_sub_f32(&a.matrix, &b.matrix, &result.matrix);

    return result;
}

template<std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
    requires MatrixCouldMultiplied<rowa, cola, rowb, colb>
ArmMatrix<float, rowa, colb>& mul(ArmMatrix<float, rowa, colb>& result,
                                  const ArmMatrix<float, rowa, cola>& a,
                                  const ArmMatrix<float, rowb, colb>& b)
{
    arm_mat_mult_f32(&a.matrix, &b.matrix, &result.matrix);

    return result;
}

template<std::size_t row, std::size_t col>
ArmMatrix<float, row, col>& ArmMatrix<float, row, col>::multiply(ArmMatrix& result,
                                                                 const ArmMatrix& a,
                                                                 const float scalar)
{
    arm_mat_scale_f32(&a.matrix, scalar, &result.matrix);

    return result;
}

template<std::size_t row, std::size_t col>
ArmMatrix<float, row, col>& ArmMatrix<float, row, col>::multiply(ArmMatrix& result,
                                                                 const float scalar,
                                                                 const ArmMatrix& a)
{
    return multiply(result, a, scalar);
}

template<std::size_t row, std::size_t col>
ArmMatrix<float, col, row>& ArmMatrix<float, row, col>::transpose(ArmMatrix<float, col, row>& result,
                                                                  const ArmMatrix<float, row, col>& a)
{
    arm_mat_trans_f32(&a.matrix, &result.matrix);

    return result;
}

template<std::size_t row, std::size_t col>
auto inverse(ArmMatrix<float, row, col>& result, const ArmMatrix<float, row, col>& a)
    -> etl::enable_if_t<row == col, ArmMatrix<float, row, col>&>
{
    arm_mat_inverse_f32(&a.matrix, &result.matrix);

    return result;
}

template<std::size_t row, std::size_t col>
float ArmMatrix<float, row, col>::determinant(ArmMatrix& a)
{
    if constexpr (row != col) {
        return 0.0f;
    }

    float det;

    return det;
}

template<std::size_t row, std::size_t col>
float* ArmMatrix<float, row, col>::at(const std::size_t r, const std::size_t c)
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
float& ArmMatrix<float, row, col>::operator()(const std::size_t r, const std::size_t c)
{
    return data[(r - 1U) * col + (c - 1U)];
}

}  // namespace rmdev

#endif  // RMDEV_USE_CMSIS_DSP
