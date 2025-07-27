/**
 * @file ArmMatrix_implements.cppm
 * @module rmdev.util.ArmMatrix:implements
 * @author 杜以成
 * @date 2025-06-12
 * @brief CMSIS-DSP 矩阵运算封装 - 实现
 */

module;

#include <cstring>
#include <initializer_list>
#include <array>

#include "arm_math.h"

#include "rmdev/concepts.hpp"

export module rmdev.util.ArmMatrix:implements;
import :traits;
import :interface;

import rmdev.util.MatrixType;
import rmdev.util.math;

export namespace rmdev {

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr ArmMatrix<Type, row, col>::ArmMatrix()
{
    ArmMatrixTraits<Type>::init(&matrix, row, col, this->data.data());
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr ArmMatrix<Type, row, col>::ArmMatrix(const ArmMatrix& other)
{
    this->data = other.data;

    ArmMatrixTraits<Type>::init(&matrix, row, col, this->data.data());
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr ArmMatrix<Type, row, col>::ArmMatrix(const MatrixType type)
{
    if constexpr (row == col) {
        switch (type) {
        case MatrixType::Normal:
        case MatrixType::Zero:
            break;

        case MatrixType::E:
            for (std::size_t i = 0, j = 0; i < row && j < col; i++, j++) {
                this->data[i * col + j] = static_cast<Type>(1);
            }
            break;

        case MatrixType::One:
            this->data.fill(static_cast<Type>(1));
            break;

        default:
            break;
        }
    }
    else {
        switch (type) {
        case MatrixType::Normal:
        case MatrixType::Zero:
        case MatrixType::E:
            break;

        case MatrixType::One:
            this->data.fill(static_cast<Type>(1));
            break;

        default:
            break;
        }
    }

    ArmMatrixTraits<Type>::init(&matrix, row, col, this->data.data());
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr ArmMatrix<Type, row, col>::ArmMatrix(const Type mat_data[row * col])
{
    std::copy(mat_data, mat_data + row * col, this->data.data());
    ArmMatrixTraits<Type>::init(&matrix, row, col, this->data.data());
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr ArmMatrix<Type, row, col>::ArmMatrix(const Type mat_data[row][col])
{
    const Type* p_data = &mat_data[0][0];
    std::copy(p_data, p_data + row * col, this->data.data());
    ArmMatrixTraits<Type>::init(&matrix, row, col, this->data.data());
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr ArmMatrix<Type, row, col>::ArmMatrix(std::initializer_list<Type> mat_data)
{
    std::copy(mat_data.begin(), mat_data.end(), this->data.begin());
    ArmMatrixTraits<Type>::init(&matrix, row, col, this->data.data());
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr ArmMatrix<Type, row, col>::ArmMatrix(std::initializer_list<std::initializer_list<Type>> mat_data)
{
    std::size_t i = 0U;
    for (const auto& row_data : mat_data) {
        std::copy(row_data.begin(), row_data.end(), this->data.begin() + i * col);
        ++i;
    }
    ArmMatrixTraits<Type>::init(&matrix, row, col, this->data.data());
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr Type* ArmMatrix<Type, row, col>::at(const std::size_t r, const std::size_t c)
{
    if (r < 1U || c < 1U) {
        return nullptr;
    }
    if ((r - 1U) >= row || (c - 1U) >= col) {
        return nullptr;
    }

    return &data[(r - 1U) * col + (c - 1U)];
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr const Type* ArmMatrix<Type, row, col>::at(const std::size_t r, const std::size_t c) const
{
    return const_cast<ArmMatrix*>(this)->at(r, c);
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr Type& ArmMatrix<Type, row, col>::operator()(const std::size_t r, const std::size_t c)
{
    return data[(r - 1U) * col + (c - 1U)];
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr const Type& ArmMatrix<Type, row, col>::operator()(const std::size_t r, const std::size_t c) const
{
    return const_cast<ArmMatrix*>(this)->operator()(r, c);
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr Type ArmMatrix<Type, row, col>::det() const
{
    if constexpr (row != col) {
        return 0;
    }

    Type det;  // todo 待完成行列式计算方法

    return det;
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr void ArmMatrix<Type, row, col>::fill(Type value)
{
    data.fill(value);
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr void ArmMatrix<Type, row, col>::clear()
{
    this->fill(static_cast<Type>(0));
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr ArmMatrix<Type, row, col>& ArmMatrix<Type, row, col>::operator=(const ArmMatrix& other)
{
    if (this != &other) {
        std::copy(other.data.begin(), other.data.end(), this->data.begin());
    }
    return *this;
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr ArmMatrix<Type, row, col>& ArmMatrix<Type, row, col>::operator=(std::initializer_list<Type> mat_data)
{
    std::copy(mat_data.begin(), mat_data.end(), this->data.begin());
    return *this;
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr ArmMatrix<Type, row, col>& ArmMatrix<Type, row, col>::operator=(
    std::initializer_list<std::initializer_list<Type>> mat_data)
{
    std::size_t i = 0U;
    for (const auto& row_data : mat_data) {
        std::copy(row_data.begin(), row_data.end(), this->data.begin() + i * col);
        ++i;
    }

    return *this;
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr bool ArmMatrix<Type, row, col>::operator==(const ArmMatrix& other) const
{
    return this->equ(other);
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr bool ArmMatrix<Type, row, col>::operator==(std::initializer_list<Type> mat_data) const
{
    return this->equ(mat_data);
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr bool ArmMatrix<Type, row, col>::operator==(std::initializer_list<std::initializer_list<Type>> mat_data) const
{
    return this->equ(mat_data);
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr bool ArmMatrix<Type, row, col>::equ(const ArmMatrix& other) const
{
    return std::equal(this->data.begin(),
                      this->data.end(),
                      other.data.begin(),
                      other.data.end(),
                      [](const auto a, const auto b) -> bool { return weakEqu(a, b); });
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr bool ArmMatrix<Type, row, col>::equ(std::initializer_list<Type> mat_data) const
{
    return std::equal(this->data.begin(),
                      this->data.end(),
                      mat_data.begin(),
                      mat_data.end(),
                      [](const auto a, const auto b) -> bool { return weakEqu(a, b); });
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr bool ArmMatrix<Type, row, col>::equ(std::initializer_list<std::initializer_list<Type>> mat_data) const
{
    std::size_t i = 0U;
    for (const auto& row_data : mat_data) {
        if (std::equal(row_data.begin(),
                       row_data.end(),
                       this->data.begin() + i * col,
                       this->data.begin() + i * col + col,
                       [](const auto a, const auto b) -> bool { return weakEqu(a, b); }) == false) {
            return false;
        }
        i++;
    }

    return true;
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr bool ArmMatrix<Type, row, col>::equ(const ArmMatrix& other, const Type error) const
{
    return std::equal(this->data.begin(),
                      this->data.end(),
                      other.data.begin(),
                      other.data.end(),
                      [error](const auto a, const auto b) -> bool { return weakEqu(a, b, error); });
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr bool ArmMatrix<Type, row, col>::equ(std::initializer_list<Type> mat_data, const Type error) const
{
    return std::equal(this->data.begin(),
                      this->data.end(),
                      mat_data.begin(),
                      mat_data.end(),
                      [error](const auto a, const auto b) -> bool { return weakEqu(a, b, error); });
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr bool ArmMatrix<Type, row, col>::equ(std::initializer_list<std::initializer_list<Type>> mat_data,
                                              const Type error) const
{
    std::size_t i = 0U;
    for (const auto& row_data : mat_data) {
        if (std::equal(row_data.begin(),
                       row_data.end(),
                       this->data.begin() + i * col,
                       this->data.begin() + i * col + col,
                       [error](const auto a, const auto b) -> bool { return weakEqu(a, b, error); }) == false) {
            return false;
        }
        i++;
    }

    return true;
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr bool ArmMatrix<Type, row, col>::operator!=(const ArmMatrix& other) const
{
    return !(this->operator==(other));
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr bool ArmMatrix<Type, row, col>::operator!=(std::initializer_list<Type> mat_data) const
{
    return !(this->operator==(mat_data));
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr bool ArmMatrix<Type, row, col>::operator!=(std::initializer_list<std::initializer_list<Type>> mat_data) const
{
    return !(this->operator==(mat_data));
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr bool ArmMatrix<Type, row, col>::notequ(const ArmMatrix& other) const
{
    return !equ(other);
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr bool ArmMatrix<Type, row, col>::notequ(std::initializer_list<Type> mat_data) const
{
    return !(this->equ(mat_data));
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr bool ArmMatrix<Type, row, col>::notequ(std::initializer_list<std::initializer_list<Type>> mat_data) const
{
    return !(this->equ(mat_data));
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr bool ArmMatrix<Type, row, col>::notequ(const ArmMatrix& other, const Type error) const
{
    return !equ(other, error);
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr bool ArmMatrix<Type, row, col>::notequ(std::initializer_list<Type> mat_data, const Type error) const
{
    return !(this->equ(mat_data, error));
}

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
constexpr bool ArmMatrix<Type, row, col>::notequ(std::initializer_list<std::initializer_list<Type>> mat_data,
                                                 const Type error) const
{
    return !(this->equ(mat_data, error));
}

/**
 * 矩阵相加
 * @tparam Type 矩阵的数据类型
 * @tparam row 矩阵行数
 * @tparam col 矩阵列数
 * @param[out] result 存储计算结果的矩阵
 * @param a 第一个矩阵
 * @param b 第二个矩阵
 * @return 计算结果
 */
template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
ArmMatrix<Type, row, col>& add(ArmMatrix<Type, row, col>& result,
                               const ArmMatrix<Type, row, col>& a,
                               const ArmMatrix<Type, row, col>& b)
{
    ArmMatrixTraits<Type>::add(&a.matrix, &b.matrix, &result.matrix);

    return result;
}

/**
 * 矩阵相减
 * @tparam Type 矩阵的数据类型
 * @tparam row 矩阵行数
 * @tparam col 矩阵列数
 * @param[out] result 存储计算结果的矩阵
 * @param a 第一个矩阵
 * @param b 第二个矩阵
 * @return 计算结果
 */
template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
ArmMatrix<Type, row, col>& sub(ArmMatrix<Type, row, col>& result,
                               const ArmMatrix<Type, row, col>& a,
                               const ArmMatrix<Type, row, col>& b)
{
    ArmMatrixTraits<Type>::sub(&a.matrix, &b.matrix, &result.matrix);

    return result;
}

/**
 * 矩阵相乘
 * @tparam Type 矩阵的数据类型
 * @tparam rowa 左侧矩阵的行数
 * @tparam cola 左侧矩阵的列数
 * @tparam rowb 右侧矩阵的行数
 * @tparam colb 右侧矩阵的列数
 * @param[out] result 存储计算结果的矩阵
 * @param a 左侧的矩阵
 * @param b 右侧的矩阵
 * @return 计算结果
 */
template<typename Type, std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
    requires ArithmeticType<Type> && MatrixCouldMultiplied<rowa, cola, rowb, colb>
ArmMatrix<Type, rowa, colb>& mul(ArmMatrix<Type, rowa, colb>& result,
                                 const ArmMatrix<Type, rowa, cola>& a,
                                 const ArmMatrix<Type, rowb, colb>& b)
{
    ArmMatrixTraits<Type>::mult(&a.matrix, &b.matrix, &result.matrix);

    return result;
}

/**
 * 矩阵数乘
 * @tparam Type 矩阵的数据类型
 * @tparam row 矩阵行数
 * @tparam col 矩阵列数
 * @param[out] result 存储计算结果的矩阵
 * @param a 矩阵
 * @param scalar 乘数
 * @return 计算结果
 */
template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
ArmMatrix<Type, row, col>& mul(ArmMatrix<Type, row, col>& result, const ArmMatrix<Type, row, col>& a, const Type scalar)
{
    ArmMatrixTraits<Type>::scale(&a.matrix, scalar, &result.matrix);

    return result;
}

/**
 * 矩阵数乘
 * @tparam Type 矩阵的数据类型
 * @tparam row 矩阵行数
 * @tparam col 矩阵列数
 * @param[out] result 存储计算结果的矩阵
 * @param scalar 乘数
 * @param a 矩阵
 * @return 计算结果
 */
template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
ArmMatrix<Type, row, col>& mul(ArmMatrix<Type, row, col>& result, const Type scalar, const ArmMatrix<Type, row, col>& a)
{
    return mul(result, a, scalar);
}

/**
 * 矩阵转置
 * @tparam Type 矩阵的数据类型
 * @tparam row 待转置的矩阵行数
 * @tparam col 待转置的矩阵列数
 * @param[out] result 存储计算结果的矩阵
 * @param a 待转置的矩阵
 * @return 计算结果
 */
template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
ArmMatrix<Type, col, row>& trans(ArmMatrix<Type, col, row>& result, const ArmMatrix<Type, row, col>& a)
{
    ArmMatrixTraits<Type>::trans(&a.matrix, &result.matrix);

    return result;
}

/**
 * 求逆矩阵
 * @attention 计算结束后，待求逆的矩阵会变成单位矩阵（原数据会丢失）
 * @tparam Type 矩阵的数据类型
 * @tparam row 待求逆的矩阵行数
 * @tparam col 待求逆的矩阵列数
 * @param[out] result 存储计算结果的矩阵
 * @param a 待求逆的矩阵
 * @return 计算结果的地址。如果矩阵不可逆，返回 nullptr
 */
template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type> && SquareMatrix<row, col>
ArmMatrix<Type, row, col>* inv(ArmMatrix<Type, row, col>& result, ArmMatrix<Type, row, col>& a)
{
    if (ArmMatrixTraits<Type>::inverse(&a.matrix, &result.matrix) == ARM_MATH_SINGULAR) {
        return nullptr;  // 矩阵不可逆
    }

    return &result;
}

/**
 * 求逆矩阵（待求逆的矩阵的原数据不会丢失，但会多出两次拷贝的开销）
 * @tparam Type 矩阵的数据类型
 * @tparam row 待求逆的矩阵行数
 * @tparam col 待求逆的矩阵列数
 * @param[out] result 存储计算结果的矩阵
 * @param a 待求逆的矩阵
 * @return 计算结果的地址。如果矩阵不可逆，返回 nullptr
 */
template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type> && SquareMatrix<row, col>
ArmMatrix<Type, row, col>* invKeep(ArmMatrix<Type, row, col>& result, const ArmMatrix<Type, row, col>& a)
{
    const std::array<Type, row * col> origin_data = a.data;

    if (ArmMatrixTraits<Type>::inverse(&a.matrix, &result.matrix) == ARM_MATH_SINGULAR) {
        return nullptr;  // 矩阵不可逆
    }

    a.data = origin_data;

    return &result;
}

/**
 * 矩阵除以数值
 * @tparam Type 矩阵的数据类型
 * @tparam row 矩阵的行数
 * @tparam col 矩阵的列数
 * @param[out] result 存储计算结果的矩阵
 * @param a 被除的矩阵
 * @param scalar 除数
 * @return 计算结果的地址。如果除数为零，返回 nullptr
 */
template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
ArmMatrix<Type, row, col>* div(ArmMatrix<Type, row, col>& result, const ArmMatrix<Type, row, col>& a, const Type scalar)
{
    const Type scalar_inv = static_cast<Type>(1) / scalar;

    if (std::isinf(scalar_inv)) {
        return nullptr;  // 除数为零
    }

    ArmMatrixTraits<Type>::scale(&a.matrix, scalar_inv, &result.matrix);

    return &result;
}

/**
 * 数值除以矩阵
 * @attention 计算结束后，这个矩阵会变成单位矩阵（原数据会丢失）
 * @tparam Type 矩阵的数据类型
 * @tparam row 矩阵的行数
 * @tparam col 矩阵的列数
 * @param[out] result 存储计算结果的矩阵
 * @param scalar 被除数
 * @param a 作为除数的矩阵
 * @return 计算结果的地址。如果矩阵不可逆，返回 nullptr
 */
template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type> && SquareMatrix<row, col>
ArmMatrix<Type, row, col>* div(ArmMatrix<Type, row, col>& result, const Type scalar, ArmMatrix<Type, row, col>& a)
{
    if (inv(result, a) == nullptr) {
        return nullptr;  // 矩阵不可逆
    }

    ArmMatrixTraits<Type>::scale(&result.matrix, scalar, &result.matrix);

    return &result;
}

/**
 * 数值除以矩阵（这个矩阵的原数据不会丢失，但会多出两次拷贝的开销）
 * @tparam Type 矩阵的数据类型
 * @tparam row 矩阵的行数
 * @tparam col 矩阵的列数
 * @param[out] result 存储计算结果的矩阵
 * @param scalar 被除数
 * @param a 作为除数的矩阵
 * @return 计算结果的地址。如果矩阵不可逆，返回 nullptr
 */
template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type> && SquareMatrix<row, col>
ArmMatrix<Type, row, col>* divKeep(ArmMatrix<Type, row, col>& result,
                                   const Type scalar,
                                   const ArmMatrix<Type, row, col>& a)
{
    if (invKeep(result, a) == nullptr) {
        return nullptr;  // 矩阵不可逆
    }

    ArmMatrixTraits<Type>::scale(&result.matrix, scalar, &result.matrix);

    return &result;
}

/**
 * 矩阵除以矩阵
 * @attention 计算结束后，作为除数的矩阵（b）会变成单位矩阵（原数据会丢失）
 * @tparam Type 矩阵的数据类型
 * @tparam rowa 作为被除数的矩阵的行数
 * @tparam cola 作为被除数的矩阵的列数
 * @tparam rowb 作为除数的矩阵的行数
 * @tparam colb 作为除数的矩阵的列数
 * @param[out] result 存储计算结果的矩阵
 * @param a 作为被除数的矩阵
 * @param b 作为除数的矩阵
 * @return 计算结果的地址。如果作为除数的矩阵不可逆，返回 nullptr
 */
template<typename Type, std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
    requires ArithmeticType<Type> && SquareMatrix<rowb, colb> && MatrixCouldMultiplied<rowa, cola, rowb, colb>
ArmMatrix<Type, rowa, colb>* div(ArmMatrix<Type, rowa, colb>& result,
                                 const ArmMatrix<Type, rowa, cola>& a,
                                 ArmMatrix<Type, rowb, colb>& b)
{
    if (inv(result, b) == nullptr) {
        return nullptr;  // 矩阵不可逆
    }

    ArmMatrixTraits<Type>::mult(&a.matrix, &result.matrix, &result.matrix);

    return &result;
}

/**
 * 矩阵除以矩阵（作为除数的矩阵（b）的原数据不会丢失，但会多出两次拷贝的开销）
 * @tparam Type 矩阵的数据类型
 * @tparam rowa 作为被除数的矩阵的行数
 * @tparam cola 作为被除数的矩阵的列数
 * @tparam rowb 作为除数的矩阵的行数
 * @tparam colb 作为除数的矩阵的列数
 * @param[out] result 存储计算结果的矩阵
 * @param a 作为被除数的矩阵
 * @param b 作为除数的矩阵
 * @return 计算结果的地址。如果作为除数的矩阵不可逆，返回 nullptr
 */
template<typename Type, std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
    requires ArithmeticType<Type> && SquareMatrix<rowb, colb> && MatrixCouldMultiplied<rowa, cola, rowb, colb>
ArmMatrix<Type, rowa, colb>* divKeep(ArmMatrix<Type, rowa, colb>& result,
                                     const ArmMatrix<Type, rowa, cola>& a,
                                     const ArmMatrix<Type, rowb, colb>& b)
{
    if (invKeep(result, b) == nullptr) {
        return nullptr;  // 矩阵不可逆
    }

    ArmMatrixTraits<Type>::mult(&a.matrix, &result.matrix, &result.matrix);

    return &result;
}

}  // namespace rmdev
