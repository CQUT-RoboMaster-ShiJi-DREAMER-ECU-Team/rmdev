/**
 * @file cmsis_dsp_mat_wrapper.cppm
 * @module rmdev.util.Matrix:CMSIS_DSP
 * @author 杜以成
 * @date 2025-06-03
 * @brief CMSIS-DSP 矩阵运算封装
 */

#if RMDEV_USE_CMSIS_DSP

module;

#include <cstddef>
#include <cstring>
#include <cmath>
#include <type_traits>
#include <initializer_list>
#include <algorithm>

#include "arm_math.h"

#include "rmdev/concepts.hpp"

export module rmdev.util.Matrix:CMSIS_DSP;

import rmdev.util.math;

// ================ declares ================
export namespace rmdev {

/**
 * ArmMatrixBase 是一个基类模板，用于实现 CMSIS-DSP 矩阵的基本操作。
 * @tparam Type 数据类型
 * @tparam row 矩阵行数
 * @tparam col 矩阵列数
 */
template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
class ArmMatrixBase
{
public:
    /**
     * 获取矩阵的第 r 行 c 列的元素（带有边界检查，如果越界，返回 nullptr）
     * @param r 第几行（从 1 开始计数）
     * @param c 第几列（从 1 开始计数）
     * @return 第 r 行 c 列的元素的地址（如果越界，返回 nullptr）
     */
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

    /**
     * 获取矩阵的第 r 行 c 列的元素 - 带有 const 修饰（带有边界检查，如果越界，返回 nullptr）
     * @param r 第几行（从 1 开始计数）
     * @param c 第几列（从 1 开始计数）
     * @return 第 r 行 c 列的元素的常量地址（如果越界，返回 nullptr）
     */
    [[nodiscard]] const Type* at(const std::size_t r, const std::size_t c) const
    {
        return const_cast<ArmMatrixBase*>(this)->at(r, c);
    }

    /**
     * 获取矩阵的第 r 行 c 列的元素（没有边界检查）
     * @param r 第几行（从 1 开始计数）
     * @param c 第几列（从 1 开始计数）
     * @return 第 r 行 c 列的元素的引用
     */
    [[nodiscard]] Type& operator()(const std::size_t r, const std::size_t c)
    {
        return data[(r - 1U) * col + (c - 1U)];
    }

    /**
     * 获取矩阵的第 r 行 c 列的元素 - 带有 const 修饰（没有边界检查）
     * @param r 第几行（从 1 开始计数）
     * @param c 第几列（从 1 开始计数）
     * @return 第 r 行 c 列的元素的常量引用
     */
    [[nodiscard]] const Type& operator()(const std::size_t r, const std::size_t c) const
    {
        return const_cast<ArmMatrixBase*>(this)->operator()(r, c);
    }

    /**
     * 计算矩阵的行列式。
     * @return 行列式计算结果（如果不是方阵，则返回 0）
     */
    [[nodiscard]] Type det() const
    {
        if constexpr (row != col) {
            return 0;
        }

        Type det;  // todo 待完成行列式计算方法

        return det;
    }

protected:
    ArmMatrixBase() = default;
    ~ArmMatrixBase() = default;

    /// 存储的矩阵数据
    Type data[row * col];
};

/**
 * ArmMatrix 矩阵类，用于封装 CMSIS-DSP 的矩阵操作。
 * @tparam Type 数据类型
 * @tparam row 行数
 * @tparam col 列数
 */
template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
class ArmMatrix;

/**
 * ArmMatrix<float, row, col> 是 ArmMatrix 的特化版本，专门用于处理 float 类型的矩阵。
 * @tparam row 行数
 * @tparam col 列数
 */
template<std::size_t row, std::size_t col>
class ArmMatrix<float, row, col> : public ArmMatrixBase<float, row, col>
{
public:
    static_assert(std::is_same_v<float, float32_t>);

    using Type = float;
    using ArmMatrixType = arm_matrix_instance_f32;

    // 由于 CMSIS-DSP 的矩阵加法函数需要分配一个新的矩阵，常规运算符难以实现，因此删除基本运算符
    ArmMatrix& operator+(const ArmMatrix& other) = delete;
    ArmMatrix& operator-(const ArmMatrix& other) = delete;
    ArmMatrix& operator*(const ArmMatrix& other) = delete;
    ArmMatrix& operator/(const ArmMatrix& other) = delete;

    ArmMatrix&& trans() = delete;

    /**
     * 默认构造函数
     */
    ArmMatrix();

    /**
     * 拷贝构造函数
     * @param other 另一个 ArmMatrix 矩阵
     */
    ArmMatrix(const ArmMatrix& other);

    /**
     * 通过一维数组构造矩阵
     * @attention 请确保这个数组的大小大于等于 row * col，否则会导致数组越界访问
     * @param mat_data 含有矩阵数据的一维数组
     */
    explicit ArmMatrix(const Type mat_data[row * col]);

    /**
     * 通过二维数组构造矩阵
     * @attention 请确保这个数组的大小大于等于 row * col，否则会导致数组越界访问
     * @param mat_data 含有矩阵数据的二维数组
     */
    explicit ArmMatrix(const Type mat_data[row][col]);

    /**
     * 一维列表初始化
     * @param mat_data 初始化列表
     */
    ArmMatrix(std::initializer_list<Type> mat_data);

    /**
     * 二维列表初始化
     * @param mat_data 二维初始化列表
     */
    ArmMatrix(std::initializer_list<std::initializer_list<Type>> mat_data);

    /**
     * 赋值运算符
     * @param other 另一个 ArmMatrix 矩阵
     * @return 赋值后的 ArmMatrix 矩阵的引用
     */
    ArmMatrix& operator=(const ArmMatrix& other);

    /**
     * 判断两个矩阵是否相等（浮点数误差值使用默认的）
     * @param other 另一个矩阵的引用
     * @return 两个矩阵是否相等
     */
    bool operator==(const ArmMatrix& other) const;

    /**
     * 判断两个矩阵是否相等（浮点数误差值使用默认的）
     * @param other 另一个矩阵的引用
     * @return 两个矩阵是否相等
     */
    bool equ(const ArmMatrix& other) const;

    /**
     * 判断两个矩阵是否相等
     * @param other 另一个矩阵的引用
     * @param error 允许的误差（a、b 之差的绝对值小于这个值即认为相等）
     * @return 两个矩阵是否相等
     */
    bool equ(const ArmMatrix& other, Type error) const;

    /**
     * 判断两个矩阵是否不相等（浮点数误差值使用默认的）
     * @param other 另一个矩阵的引用
     * @return 两个矩阵是否不相等
     */
    bool operator!=(const ArmMatrix& other) const;

    /**
     * 判断两个矩阵是否不相等（浮点数误差值使用默认的）
     * @param other 另一个矩阵的引用
     * @return 两个矩阵是否不相等
     */
    bool notequ(const ArmMatrix& other) const;

    /**
     * 判断两个矩阵是否不相等
     * @param other 另一个矩阵的引用
     * @param error 允许的误差（a、b 之差的绝对值小于这个值即认为相等）
     * @return 两个矩阵是否不相等
     */
    bool notequ(const ArmMatrix& other, Type error) const;

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

    template<std::size_t row_, std::size_t col_>
    friend ArmMatrix<Type, row_, col_>* div(ArmMatrix<Type, row_, col_>& result,
                                            const ArmMatrix<Type, row_, col_>& a,
                                            Type scalar);

    template<std::size_t row_, std::size_t col_>
        requires SquareMatrix<row_, col_>
    friend ArmMatrix<Type, row_, col_>* div(ArmMatrix<Type, row_, col_>& result,
                                            Type scalar,
                                            const ArmMatrix<Type, row_, col_>& a);

    template<std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
        requires SquareMatrix<rowb, colb> && MatrixCouldMultiplied<rowa, cola, rowb, colb>
    friend ArmMatrix<Type, rowa, colb>* div(ArmMatrix<Type, rowa, colb>& result,
                                            const ArmMatrix<Type, rowa, cola>& a,
                                            const ArmMatrix<Type, rowb, colb>& b);

private:
    ArmMatrixType matrix{};  ///< CMSIS-DSP 矩阵实例
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
bool ArmMatrix<float, row, col>::equ(const ArmMatrix& other) const
{
    return std::equal(this->data, this->data + row * col, other.data, [](const auto a, const auto b) -> bool {
        return floatEqu(a, b);
    });
}

template<std::size_t row, std::size_t col>
bool ArmMatrix<float, row, col>::equ(const ArmMatrix& other, const Type error) const
{
    return std::equal(this->data, this->data + row * col, other.data, [error](const auto a, const auto b) -> bool {
        return floatEqu(a, b, error);
    });
}

template<std::size_t row, std::size_t col>
bool ArmMatrix<ImplType, row, col>::operator!=(const ArmMatrix& other) const
{
    return !(this->operator==(other));
}

template<std::size_t row, std::size_t col>
bool ArmMatrix<ImplType, row, col>::notequ(const ArmMatrix& other) const
{
    return !equ(other);
}

template<std::size_t row, std::size_t col>
bool ArmMatrix<ImplType, row, col>::notequ(const ArmMatrix& other, const Type error) const
{
    return !equ(other, error);
}

/**
 * 矩阵相加
 * @tparam row 矩阵行数
 * @tparam col 矩阵列数
 * @param result 存储计算结果的矩阵
 * @param a 第一个矩阵
 * @param b 第二个矩阵
 * @return 计算结果
 */
template<std::size_t row, std::size_t col>
ArmMatrix<ImplType, row, col>& add(ArmMatrix<ImplType, row, col>& result,
                                   const ArmMatrix<ImplType, row, col>& a,
                                   const ArmMatrix<ImplType, row, col>& b)
{
    arm_mat_add_f32(&a.matrix, &b.matrix, &result.matrix);

    return result;
}

/**
 * 矩阵相减
 * @tparam row 矩阵行数
 * @tparam col 矩阵列数
 * @param result 存储计算结果的矩阵
 * @param a 第一个矩阵
 * @param b 第二个矩阵
 * @return 计算结果
 */
template<std::size_t row, std::size_t col>
ArmMatrix<ImplType, row, col>& sub(ArmMatrix<ImplType, row, col>& result,
                                   const ArmMatrix<ImplType, row, col>& a,
                                   const ArmMatrix<ImplType, row, col>& b)
{
    arm_mat_sub_f32(&a.matrix, &b.matrix, &result.matrix);

    return result;
}

/**
 * 矩阵相乘
 * @tparam rowa 左侧矩阵的行数
 * @tparam cola 左侧矩阵的列数
 * @tparam rowb 右侧矩阵的行数
 * @tparam colb 右侧矩阵的列数
 * @param result 存储计算结果的矩阵
 * @param a 左侧的矩阵
 * @param b 右侧的矩阵
 * @return 计算结果
 */
template<std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
    requires MatrixCouldMultiplied<rowa, cola, rowb, colb>
ArmMatrix<ImplType, rowa, colb>& mul(ArmMatrix<ImplType, rowa, colb>& result,
                                     const ArmMatrix<ImplType, rowa, cola>& a,
                                     const ArmMatrix<ImplType, rowb, colb>& b)
{
    arm_mat_mult_f32(&a.matrix, &b.matrix, &result.matrix);

    return result;
}

/**
 * 矩阵数乘
 * @tparam row 矩阵行数
 * @tparam col 矩阵列数
 * @param result 存储计算结果的矩阵
 * @param a 矩阵
 * @param scalar 乘数
 * @return 计算结果
 */
template<std::size_t row, std::size_t col>
ArmMatrix<ImplType, row, col>& mul(ArmMatrix<ImplType, row, col>& result,
                                   const ArmMatrix<ImplType, row, col>& a,
                                   const ImplType scalar)
{
    arm_mat_scale_f32(&a.matrix, scalar, &result.matrix);

    return result;
}

/**
 * 矩阵数乘
 * @tparam row 矩阵行数
 * @tparam col 矩阵列数
 * @param result 存储计算结果的矩阵
 * @param scalar 乘数
 * @param a 矩阵
 * @return 计算结果
 */
template<std::size_t row, std::size_t col>
ArmMatrix<ImplType, row, col>& mul(ArmMatrix<ImplType, row, col>& result,
                                   const ImplType scalar,
                                   const ArmMatrix<ImplType, row, col>& a)
{
    return mul(result, a, scalar);
}

/**
 * 矩阵转置
 * @tparam row 待转置的矩阵行数
 * @tparam col 待转置的矩阵列数
 * @param result 存储计算结果的矩阵
 * @param a 待转置的矩阵
 * @return 计算结果
 */
template<std::size_t row, std::size_t col>
ArmMatrix<ImplType, col, row>& trans(ArmMatrix<ImplType, col, row>& result, const ArmMatrix<ImplType, row, col>& a)
{
    arm_mat_trans_f32(&a.matrix, &result.matrix);

    return result;
}

/**
 * 求逆矩阵
 * @tparam row 待求逆的矩阵行数
 * @tparam col 待求逆的矩阵列数
 * @param result 存储计算结果的矩阵
 * @param a 待求逆的矩阵
 * @return 计算结果的地址。如果矩阵不可逆，返回 nullptr
 */
template<std::size_t row, std::size_t col>
    requires SquareMatrix<row, col>
ArmMatrix<ImplType, row, col>* inv(ArmMatrix<ImplType, row, col>& result, const ArmMatrix<ImplType, row, col>& a)
{
    if (arm_mat_inverse_f32(&a.matrix, &result.matrix) == ARM_MATH_SINGULAR) {
        return nullptr;  // 矩阵不可逆
    }

    return &result;
}

/**
 * 矩阵除以数值
 * @tparam row 矩阵的行数
 * @tparam col 矩阵的列数
 * @param result 存储计算结果的矩阵
 * @param a 被除的矩阵
 * @param scalar 除数
 * @return 计算结果的地址。如果除数为零，返回 nullptr
 */
template<std::size_t row, std::size_t col>
ArmMatrix<ImplType, row, col>* div(ArmMatrix<ImplType, row, col>& result,
                                   const ArmMatrix<ImplType, row, col>& a,
                                   const ImplType scalar)
{
    const ImplType scalar_inv = static_cast<ImplType>(1) / scalar;

    if (std::isinf(scalar_inv)) {
        return nullptr;  // 除数为零
    }

    arm_mat_scale_f32(&a.matrix, scalar_inv, &result.matrix);

    return &result;
}

/**
 * 数值除以矩阵
 * @tparam row 矩阵的行数
 * @tparam col 矩阵的列数
 * @param result 存储计算结果的矩阵
 * @param scalar 被除数
 * @param a 作为除数的矩阵
 * @return 计算结果的地址。如果矩阵不可逆，返回 nullptr
 */
template<std::size_t row, std::size_t col>
    requires SquareMatrix<row, col>
ArmMatrix<ImplType, row, col>* div(ArmMatrix<ImplType, row, col>& result,
                                   const ImplType scalar,
                                   const ArmMatrix<ImplType, row, col>& a)
{
    if (arm_mat_inverse_f32(&a.matrix, &result.matrix) == ARM_MATH_SINGULAR) {
        return nullptr;  // 矩阵不可逆
    }

    arm_mat_scale_f32(&result.matrix, scalar, &result.matrix);

    return &result;
}

/**
 * 矩阵除以矩阵
 * @tparam rowa 作为被除数的矩阵的行数
 * @tparam cola 作为被除数的矩阵的列数
 * @tparam rowb 作为除数的矩阵的行数
 * @tparam colb 作为除数的矩阵的列数
 * @param result 存储计算结果的矩阵
 * @param a 作为被除数的矩阵
 * @param b 作为除数的矩阵
 * @return 计算结果的地址。如果作为除数的矩阵不可逆，返回 nullptr
 */
template<std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
    requires SquareMatrix<rowb, colb> && MatrixCouldMultiplied<rowa, cola, rowb, colb>
ArmMatrix<ImplType, rowa, colb>* div(ArmMatrix<ImplType, rowa, colb>& result,
                                     const ArmMatrix<ImplType, rowa, cola>& a,
                                     const ArmMatrix<ImplType, rowb, colb>& b)
{
    if (arm_mat_inverse_f32(&b.matrix, &result.matrix) == ARM_MATH_SINGULAR) {
        return nullptr;  // 矩阵不可逆
    }

    arm_mat_mult_f32(&a.matrix, &result.matrix, &result.matrix);

    return &result;
}

}  // namespace rmdev

// double 类型的矩阵实现
export namespace rmdev {

#undef ImplType
#define ImplType double

}  // namespace rmdev

#endif  // RMDEV_USE_CMSIS_DSP
