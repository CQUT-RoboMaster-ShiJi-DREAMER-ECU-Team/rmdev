/**
 * @file ArmMatrix_interface.cppm
 * @module rmdev.util.ArmMatrix:interface
 * @author 杜以成
 * @date 2025-06-03
 * @brief CMSIS-DSP 矩阵运算封装接口
 */

module;

#include <cstddef>
#include <initializer_list>
#include <array>

#include "arm_math.h"

#include "rmdev/concepts.hpp"

export module rmdev.util.ArmMatrix:interface;
import :traits;

import rmdev.util.MatrixType;

namespace rmdev {

/**
 * ArmMatrix 矩阵类，用于封装 CMSIS-DSP 的矩阵操作。
 * @tparam Type 数据类型
 * @tparam row 行数
 * @tparam col 列数
 */
export template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
class ArmMatrix
{
public:
    // todo 待实现这些运算符
    ArmMatrix& operator+(const ArmMatrix& other) = delete;
    ArmMatrix& operator-(const ArmMatrix& other) = delete;
    ArmMatrix& operator*(const ArmMatrix& other) = delete;
    ArmMatrix& operator/(const ArmMatrix& other) = delete;

    ArmMatrix&& trans() = delete;

    /**
     * 默认构造函数
     */
    constexpr ArmMatrix();

    /**
     * 拷贝构造函数
     * @param other 另一个 ArmMatrix 矩阵
     */
    constexpr ArmMatrix(const ArmMatrix& other);

    /**
     * 构造特殊矩阵
     * @param type 特殊矩阵类型
     */
    explicit constexpr ArmMatrix(MatrixType type);

    /**
     * 通过一维数组构造矩阵
     * @attention 请确保这个数组的大小大于等于 row * col，否则会导致数组越界访问
     * @param mat_data 含有矩阵数据的一维数组
     */
    explicit constexpr ArmMatrix(const Type mat_data[row * col]);

    /**
     * 通过二维数组构造矩阵
     * @attention 请确保这个数组的大小大于等于 row * col，否则会导致数组越界访问
     * @param mat_data 含有矩阵数据的二维数组
     */
    explicit constexpr ArmMatrix(const Type mat_data[row][col]);

    /**
     * 一维列表初始化
     * @param mat_data 初始化列表
     */
    constexpr ArmMatrix(std::initializer_list<Type> mat_data);

    /**
     * 二维列表初始化
     * @param mat_data 二维初始化列表
     */
    constexpr ArmMatrix(std::initializer_list<std::initializer_list<Type>> mat_data);

    /**
     * 获取矩阵的第 r 行 c 列的元素（带有边界检查，如果越界，返回 nullptr）
     * @param r 第几行（从 1 开始计数）
     * @param c 第几列（从 1 开始计数）
     * @return 第 r 行 c 列的元素的地址（如果越界，返回 nullptr）
     */
    [[nodiscard]] constexpr Type* at(std::size_t r, std::size_t c);

    /**
     * 获取矩阵的第 r 行 c 列的元素 - 带有 const 修饰（带有边界检查，如果越界，返回 nullptr）
     * @param r 第几行（从 1 开始计数）
     * @param c 第几列（从 1 开始计数）
     * @return 第 r 行 c 列的元素的常量地址（如果越界，返回 nullptr）
     */
    [[nodiscard]] constexpr const Type* at(std::size_t r, std::size_t c) const;

    /**
     * 获取矩阵的第 r 行 c 列的元素（没有边界检查）
     * @param r 第几行（从 1 开始计数）
     * @param c 第几列（从 1 开始计数）
     * @return 第 r 行 c 列的元素的引用
     */
    [[nodiscard]] constexpr Type& operator()(std::size_t r, std::size_t c);

    /**
     * 获取矩阵的第 r 行 c 列的元素 - 带有 const 修饰（没有边界检查）
     * @param r 第几行（从 1 开始计数）
     * @param c 第几列（从 1 开始计数）
     * @return 第 r 行 c 列的元素的常量引用
     */
    [[nodiscard]] constexpr const Type& operator()(std::size_t r, std::size_t c) const;

    /**
     * 计算矩阵的行列式。
     * @return 行列式计算结果（如果不是方阵，则返回 0）
     */
    [[nodiscard]] constexpr Type det() const;

    /**
     * 将矩阵的所有元素全部设置为给定值
     * @param value 值
     */
    constexpr void fill(Type value);

    /**
     * 将矩阵的所有元素全部清零
     */
    constexpr void clear();

    /**
     * 赋值运算符
     * @param other 另一个 ArmMatrix 矩阵
     * @return 赋值后的 ArmMatrix 矩阵的引用
     */
    constexpr ArmMatrix& operator=(const ArmMatrix& other);

    /**
     * 赋值运算符
     * @param mat_data 一个一维的初始化列表
     * @return 赋值后的 ArmMatrix 矩阵的引用
     */
    constexpr ArmMatrix& operator=(std::initializer_list<Type> mat_data);

    /**
     * 赋值运算符
     * @param mat_data 一个二维的初始化列表
     * @return 赋值后的 ArmMatrix 矩阵的引用
     */
    constexpr ArmMatrix& operator=(std::initializer_list<std::initializer_list<Type>> mat_data);

    /**
     * 判断两个矩阵是否相等（浮点数误差值使用默认的）
     * @param other 另一个矩阵的引用
     * @return 两个矩阵是否相等
     */
    constexpr bool operator==(const ArmMatrix& other) const;

    /**
     * 判断两个矩阵是否相等（浮点数误差值使用默认的）
     * @param mat_data 矩阵的一维初始化列表
     * @return 两个矩阵是否相等
     */
    constexpr bool operator==(std::initializer_list<Type> mat_data) const;

    /**
     * 判断两个矩阵是否相等（浮点数误差值使用默认的）
     * @param mat_data 矩阵的二维初始化列表
     * @return 两个矩阵是否相等
     */
    constexpr bool operator==(std::initializer_list<std::initializer_list<Type>> mat_data) const;

    /**
     * 判断两个矩阵是否相等（浮点数误差值使用默认的）
     * @param other 另一个矩阵的引用
     * @return 两个矩阵是否相等
     */
    constexpr bool equ(const ArmMatrix& other) const;

    /**
     * 判断两个矩阵是否相等（浮点数误差值使用默认的）
     * @param mat_data 矩阵的一维初始化列表
     * @return 两个矩阵是否相等
     */
    constexpr bool equ(std::initializer_list<Type> mat_data) const;

    /**
     * 判断两个矩阵是否相等（浮点数误差值使用默认的）
     * @param mat_data 矩阵的二维初始化列表
     * @return 两个矩阵是否相等
     */
    constexpr bool equ(std::initializer_list<std::initializer_list<Type>> mat_data) const;

    /**
     * 判断两个矩阵是否相等
     * @param other 另一个矩阵的引用
     * @param error 允许的误差（a、b 之差的绝对值小于这个值即认为相等）
     * @return 两个矩阵是否相等
     */
    constexpr bool equ(const ArmMatrix& other, Type error) const;

    /**
     * 判断两个矩阵是否相等
     * @param mat_data 矩阵的一维初始化列表
     * @param error 允许的误差（a、b 之差的绝对值小于这个值即认为相等）
     * @return 两个矩阵是否相等
     */
    constexpr bool equ(std::initializer_list<Type> mat_data, Type error) const;

    /**
     * 判断两个矩阵是否相等
     * @param mat_data 矩阵的二维初始化列表
     * @param error 允许的误差（a、b 之差的绝对值小于这个值即认为相等）
     * @return 两个矩阵是否相等
     */
    constexpr bool equ(std::initializer_list<std::initializer_list<Type>> mat_data, Type error) const;

    /**
     * 判断两个矩阵是否不相等（浮点数误差值使用默认的）
     * @param other 另一个矩阵的引用
     * @return 两个矩阵是否不相等
     */
    constexpr bool operator!=(const ArmMatrix& other) const;

    /**
     * 判断两个矩阵是否不相等（浮点数误差值使用默认的）
     * @param mat_data 矩阵的一维初始化列表
     * @return 两个矩阵是否不相等
     */
    constexpr bool operator!=(std::initializer_list<Type> mat_data) const;

    /**
     * 判断两个矩阵是否不相等（浮点数误差值使用默认的）
     * @param mat_data 矩阵的二维初始化列表
     * @return 两个矩阵是否不相等
     */
    constexpr bool operator!=(std::initializer_list<std::initializer_list<Type>> mat_data) const;

    /**
     * 判断两个矩阵是否不相等（浮点数误差值使用默认的）
     * @param other 另一个矩阵的引用
     * @return 两个矩阵是否不相等
     */
    constexpr bool notequ(const ArmMatrix& other) const;

    /**
     * 判断两个矩阵是否不相等（浮点数误差值使用默认的）
     * @param mat_data 矩阵的一维初始化列表
     * @return 两个矩阵是否不相等
     */
    constexpr bool notequ(std::initializer_list<Type> mat_data) const;

    /**
     * 判断两个矩阵是否不相等（浮点数误差值使用默认的）
     * @param mat_data 矩阵的二维初始化列表
     * @return 两个矩阵是否不相等
     */
    constexpr bool notequ(std::initializer_list<std::initializer_list<Type>> mat_data) const;

    /**
     * 判断两个矩阵是否不相等
     * @param other 另一个矩阵的引用
     * @param error 允许的误差（a、b 之差的绝对值小于这个值即认为相等）
     * @return 两个矩阵是否不相等
     */
    constexpr bool notequ(const ArmMatrix& other, Type error) const;

    /**
     * 判断两个矩阵是否不相等
     * @param mat_data 矩阵的一维初始化列表
     * @param error 允许的误差（a、b 之差的绝对值小于这个值即认为相等）
     * @return 两个矩阵是否不相等
     */
    constexpr bool notequ(std::initializer_list<Type> mat_data, Type error) const;

    /**
     * 判断两个矩阵是否不相等
     * @param mat_data 矩阵的二维初始化列表
     * @param error 允许的误差（a、b 之差的绝对值小于这个值即认为相等）
     * @return 两个矩阵是否不相等
     */
    constexpr bool notequ(std::initializer_list<std::initializer_list<Type>> mat_data, Type error) const;

    template<typename Type_, std::size_t row_, std::size_t col_>
        requires ArithmeticType<Type_>
    friend ArmMatrix<Type_, row_, col_>& add(ArmMatrix<Type_, row_, col_>& result,
                                             const ArmMatrix<Type_, row_, col_>& a,
                                             const ArmMatrix<Type_, row_, col_>& b);

    template<typename Type_, std::size_t row_, std::size_t col_>
        requires ArithmeticType<Type_>
    friend ArmMatrix<Type_, row_, col_>& sub(ArmMatrix<Type_, row_, col_>& result,
                                             const ArmMatrix<Type_, row_, col_>& a,
                                             const ArmMatrix<Type_, row_, col_>& b);

    template<typename Type_, std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
        requires ArithmeticType<Type_> && MatrixCouldMultiplied<rowa, cola, rowb, colb>
    friend ArmMatrix<Type_, rowa, colb>& mul(ArmMatrix<Type_, rowa, colb>& result,
                                             const ArmMatrix<Type_, rowa, cola>& a,
                                             const ArmMatrix<Type_, rowb, colb>& b);

    template<typename Type_, std::size_t row_, std::size_t col_>
        requires ArithmeticType<Type_>
    friend ArmMatrix<Type_, row_, col_>& mul(ArmMatrix<Type_, row_, col_>& result,
                                             const ArmMatrix<Type_, row_, col_>& a,
                                             Type_ scalar);

    template<typename Type_, std::size_t row_, std::size_t col_>
        requires ArithmeticType<Type_>
    friend ArmMatrix<Type_, row_, col_>& mul(ArmMatrix<Type_, row_, col_>& result,
                                             Type_ scalar,
                                             const ArmMatrix<Type_, row_, col_>& a);

    template<typename Type_, std::size_t row_, std::size_t col_>
        requires ArithmeticType<Type_>
    friend ArmMatrix<Type_, col_, row_>& trans(ArmMatrix<Type_, col_, row_>& result,
                                               const ArmMatrix<Type_, row_, col_>& a);

    template<typename Type_, std::size_t row_, std::size_t col_>
        requires ArithmeticType<Type_> && SquareMatrix<row_, col_>
    friend ArmMatrix<Type_, row_, col_>* inv(ArmMatrix<Type_, row_, col_>& result, ArmMatrix<Type_, row_, col_>& a);

    template<typename Type_, std::size_t row_, std::size_t col_>
        requires ArithmeticType<Type_> && SquareMatrix<row_, col_>
    friend ArmMatrix<Type_, row_, col_>* invKeep(ArmMatrix<Type_, row_, col_>& result,
                                                 const ArmMatrix<Type_, row_, col_>& a);

    template<typename Type_, std::size_t row_, std::size_t col_>
        requires ArithmeticType<Type_>
    friend ArmMatrix<Type_, row_, col_>* div(ArmMatrix<Type_, row_, col_>& result,
                                             const ArmMatrix<Type_, row_, col_>& a,
                                             Type_ scalar);

    template<typename Type_, std::size_t row_, std::size_t col_>
        requires ArithmeticType<Type_> && SquareMatrix<row_, col_>
    friend ArmMatrix<Type_, row_, col_>* div(ArmMatrix<Type_, row_, col_>& result,
                                             Type_ scalar,
                                             ArmMatrix<Type_, row_, col_>& a);

    template<typename Type_, std::size_t row_, std::size_t col_>
        requires ArithmeticType<Type_> && SquareMatrix<row_, col_>
    friend ArmMatrix<Type_, row_, col_>* divKeep(ArmMatrix<Type_, row_, col_>& result,
                                                 Type_ scalar,
                                                 const ArmMatrix<Type_, row_, col_>& a);

    template<typename Type_, std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
        requires ArithmeticType<Type_> && SquareMatrix<rowb, colb> && MatrixCouldMultiplied<rowa, cola, rowb, colb>
    friend ArmMatrix<Type_, rowa, colb>* div(ArmMatrix<Type_, rowa, colb>& result,
                                             const ArmMatrix<Type_, rowa, cola>& a,
                                             ArmMatrix<Type_, rowb, colb>& b);

    template<typename Type_, std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
        requires ArithmeticType<Type_> && SquareMatrix<rowb, colb> && MatrixCouldMultiplied<rowa, cola, rowb, colb>
    friend ArmMatrix<Type_, rowa, colb>* divKeep(ArmMatrix<Type_, rowa, colb>& result,
                                                 const ArmMatrix<Type_, rowa, cola>& a,
                                                 const ArmMatrix<Type_, rowb, colb>& b);

private:
    std::array<Type, row * col> data{};                          ///< 存储的矩阵数据
    typename ArmMatrixTraits<Type>::ArmMatrixInstance matrix{};  ///< Arm 矩阵实例
};

}  // namespace rmdev
