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

import rmdev.util.math;

// ================ declares ================
export namespace rmdev {

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
class ArmMatrixBase
{
public:
    [[nodiscard]] Type* at(const std::size_t r, const std::size_t c)
    {
        if (r < 1U || c < 1U) {
            return nullptr;
        }
        if ((r - 1U) >= row || (c - 1U) >= col) {
            return nullptr;
        }

        return &data[(r - 1U) * col + (c - 1U)];
    }

    [[nodiscard]] const Type* at(const std::size_t r, const std::size_t c) const
    {
        return const_cast<ArmMatrixBase*>(this)->at(r, c);
    }

    [[nodiscard]] Type& operator()(const std::size_t r, const std::size_t c)
    {
        return data[(r - 1U) * col + (c - 1U)];
    }

    [[nodiscard]] const Type& operator()(const std::size_t r, const std::size_t c) const
    {
        return const_cast<ArmMatrixBase*>(this)->operator()(r, c);
    }

    [[nodiscard]] Type det() const
    {
        if constexpr (row != col) {
            return 0.0f;
        }

        float det;

        return det;
    }

protected:
    ArmMatrixBase() = default;
    ~ArmMatrixBase() = default;

    Type data[row * col];
};

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
class ArmMatrix;

template<std::size_t row, std::size_t col>
class ArmMatrix<float, row, col> : public ArmMatrixBase<float, row, col>
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

    bool operator==(const ArmMatrix& other) const;

    bool equ(const ArmMatrix& other) const
    {
        return etl::equal(this->data, this->data + row * col, other.data, [](const auto a, const auto b) -> bool {
            return floatEqu(a, b);
        });
    }

    bool equ(const ArmMatrix& other, const Type error) const
    {
        return etl::equal(this->data, this->data + row * col, other.data, [error](const auto a, const auto b) -> bool {
            return floatEqu(a, b, error);
        });
    }

    bool operator!=(const ArmMatrix& other) const;

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

    template<std::size_t row_, std::size_t col_>
    friend ArmMatrix<Type, row_, col_>& mul(ArmMatrix<Type, row_, col_>& result,
                                            const ArmMatrix<Type, row_, col_>& a,
                                            Type scalar);

    template<std::size_t row_, std::size_t col_>
    friend ArmMatrix<Type, row_, col_>& mul(ArmMatrix<Type, row_, col_>& result,
                                            Type scalar,
                                            const ArmMatrix<Type, row_, col_>& a);

    template<std::size_t row_, std::size_t col_>
    friend ArmMatrix<Type, col_, row_>& trans(ArmMatrix<Type, col_, row_>& result,
                                              const ArmMatrix<Type, row_, col_>& a);

    template<std::size_t row_, std::size_t col_>
        requires SquareMatrix<row_, col_>
    friend ArmMatrix<Type, row_, col_>* inv(ArmMatrix<Type, row_, col_>& result, const ArmMatrix<Type, row_, col_>& a);

private:
    ArmMatrixType matrix{};
};

template<std::size_t row, std::size_t col>
class ArmMatrix<double, row, col>;  // todo 待完成其他类型的特化

}  // namespace rmdev

// ================== implements ==================

// float 类型的矩阵实现
export namespace rmdev {

#define ImplType float

template<std::size_t row, std::size_t col>
ArmMatrix<ImplType, row, col>::ArmMatrix()
{
    arm_mat_init_f32(&matrix, row, col, this->data);
}

template<std::size_t row, std::size_t col>
ArmMatrix<ImplType, row, col>::ArmMatrix(const ArmMatrix& other)
{
    std::memcpy(this->data, other.data, sizeof this->data);
    this->matrix = {.numRows = other.matrix.numRows, .numCols = other.matrix.numCols, .pData = this->data};

    arm_mat_init_f32(&matrix, row, col, this->data);
}

template<std::size_t row, std::size_t col>
ArmMatrix<ImplType, row, col>::ArmMatrix(const ImplType mat_data[row * col])
{
    std::memcpy(this->data, mat_data, sizeof this->data);
    arm_mat_init_f32(&matrix, row, col, this->data);
}

template<std::size_t row, std::size_t col>
ArmMatrix<ImplType, row, col>::ArmMatrix(const ImplType mat_data[row][col])
{
    std::memcpy(this->data, mat_data, sizeof this->data);
    arm_mat_init_f32(&matrix, row, col, this->data);
}

template<std::size_t row, std::size_t col>
ArmMatrix<ImplType, row, col>::ArmMatrix(const std::initializer_list<ImplType> mat_data)
{
    std::memcpy(this->data, mat_data.begin(), sizeof this->data);
    arm_mat_init_f32(&matrix, row, col, this->data);
}

template<std::size_t row, std::size_t col>
ArmMatrix<ImplType, row, col>::ArmMatrix(const std::initializer_list<std::initializer_list<ImplType>> mat_data)
{
    std::size_t i = 0;
    for (const auto& row_data : mat_data) {
        std::memcpy(&this->data[i * col], row_data.begin(), sizeof(ImplType) * row_data.size());
        ++i;
    }
    arm_mat_init_f32(&matrix, row, col, this->data);
}

template<std::size_t row, std::size_t col>
ArmMatrix<ImplType, row, col>& ArmMatrix<ImplType, row, col>::operator=(const ArmMatrix& other)
{
    if (this != &other) {
        std::memcpy(this->data, other.data, sizeof this->data);
    }
    return *this;
}

template<std::size_t row, std::size_t col>
bool ArmMatrix<ImplType, row, col>::operator==(const ArmMatrix& other) const
{
    return this->equ(other);
}

template<std::size_t row, std::size_t col>
bool ArmMatrix<ImplType, row, col>::operator!=(const ArmMatrix& other) const
{
    return !(this->operator==(other));
}

template<std::size_t row, std::size_t col>
ArmMatrix<ImplType, row, col>& add(ArmMatrix<ImplType, row, col>& result,
                                   const ArmMatrix<ImplType, row, col>& a,
                                   const ArmMatrix<ImplType, row, col>& b)
{
    arm_mat_add_f32(&a.matrix, &b.matrix, &result.matrix);

    return result;
}

template<std::size_t row, std::size_t col>
ArmMatrix<ImplType, row, col>& sub(ArmMatrix<ImplType, row, col>& result,
                                   const ArmMatrix<ImplType, row, col>& a,
                                   const ArmMatrix<ImplType, row, col>& b)
{
    arm_mat_sub_f32(&a.matrix, &b.matrix, &result.matrix);

    return result;
}

template<std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
    requires MatrixCouldMultiplied<rowa, cola, rowb, colb>
ArmMatrix<ImplType, rowa, colb>& mul(ArmMatrix<ImplType, rowa, colb>& result,
                                     const ArmMatrix<ImplType, rowa, cola>& a,
                                     const ArmMatrix<ImplType, rowb, colb>& b)
{
    arm_mat_mult_f32(&a.matrix, &b.matrix, &result.matrix);

    return result;
}

template<std::size_t row, std::size_t col>
ArmMatrix<ImplType, row, col>& mul(ArmMatrix<ImplType, row, col>& result,
                                   const ArmMatrix<ImplType, row, col>& a,
                                   const ImplType scalar)
{
    arm_mat_scale_f32(&a.matrix, scalar, &result.matrix);

    return result;
}

template<std::size_t row, std::size_t col>
ArmMatrix<ImplType, row, col>& mul(ArmMatrix<ImplType, row, col>& result,
                                   const ImplType scalar,
                                   const ArmMatrix<ImplType, row, col>& a)
{
    return mul(result, a, scalar);
}

template<std::size_t row, std::size_t col>
ArmMatrix<ImplType, col, row>& trans(ArmMatrix<ImplType, col, row>& result, const ArmMatrix<ImplType, row, col>& a)
{
    arm_mat_trans_f32(&a.matrix, &result.matrix);

    return result;
}

template<std::size_t row, std::size_t col>
    requires SquareMatrix<row, col>
ArmMatrix<ImplType, row, col>* inv(ArmMatrix<ImplType, row, col>& result, const ArmMatrix<ImplType, row, col>& a)
{
    if (arm_mat_inverse_f32(&a.matrix, &result.matrix) == ARM_MATH_SINGULAR) {
        return nullptr;  // 矩阵不可逆
    }

    return &result;
}

}  // namespace rmdev

// double 类型的矩阵实现
export namespace rmdev {

#undef ImplType
#define ImplType double

}  // namespace rmdev

#endif  // RMDEV_USE_CMSIS_DSP
