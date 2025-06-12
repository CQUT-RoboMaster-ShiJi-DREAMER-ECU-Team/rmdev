/**
 * @file ArmMatrix.cppm
 * @module rmdev.util.Matrix:ArmMatrix_interface
 * @author 杜以成
 * @date 2025-06-03
 * @brief CMSIS-DSP 矩阵运算封装
 */

#if RMDEV_USE_CMSIS_DSP

module;

#include <cstddef>
#include <initializer_list>
#include <array>

#include "arm_math.h"

#include "rmdev/concepts.hpp"

export module rmdev.util.Matrix:ArmMatrix_interface;
import :Type;

namespace rmdev {

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
    std::array<Type, row * col> data{};
};

/**
 * ArmMatrix 矩阵类，用于封装 CMSIS-DSP 的矩阵操作。
 * @tparam Type 数据类型
 * @tparam row 行数
 * @tparam col 列数
 */
export template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
class ArmMatrix;

/**
 * ArmMatrix<float, row, col> 是 ArmMatrix 的特化版本，专门用于处理 float 类型的矩阵。
 * @tparam row 行数
 * @tparam col 列数
 */
export template<std::size_t row, std::size_t col>
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
     * 构造特殊矩阵
     * @param type 特殊矩阵类型
     */
    explicit ArmMatrix(MatrixType type);

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
    friend ArmMatrix<Type, row_, col_>* inv(ArmMatrix<Type, row_, col_>& result, ArmMatrix<Type, row_, col_>& a);

    template<std::size_t row_, std::size_t col_>
        requires SquareMatrix<row_, col_>
    friend ArmMatrix<Type, row_, col_>* invKeep(ArmMatrix<Type, row_, col_>& result,
                                                const ArmMatrix<Type, row_, col_>& a);

    template<std::size_t row_, std::size_t col_>
    friend ArmMatrix<Type, row_, col_>* div(ArmMatrix<Type, row_, col_>& result,
                                            const ArmMatrix<Type, row_, col_>& a,
                                            Type scalar);

    template<std::size_t row_, std::size_t col_>
        requires SquareMatrix<row_, col_>
    friend ArmMatrix<Type, row_, col_>* div(ArmMatrix<Type, row_, col_>& result,
                                            Type scalar,
                                            ArmMatrix<Type, row_, col_>& a);

    template<std::size_t row_, std::size_t col_>
        requires SquareMatrix<row_, col_>
    friend ArmMatrix<Type, row_, col_>* divKeep(ArmMatrix<Type, row_, col_>& result,
                                                Type scalar,
                                                const ArmMatrix<Type, row_, col_>& a);

    template<std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
        requires SquareMatrix<rowb, colb> && MatrixCouldMultiplied<rowa, cola, rowb, colb>
    friend ArmMatrix<Type, rowa, colb>* div(ArmMatrix<Type, rowa, colb>& result,
                                            const ArmMatrix<Type, rowa, cola>& a,
                                            ArmMatrix<Type, rowb, colb>& b);

    template<std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
        requires SquareMatrix<rowb, colb> && MatrixCouldMultiplied<rowa, cola, rowb, colb>
    friend ArmMatrix<Type, rowa, colb>* divKeep(ArmMatrix<Type, rowa, colb>& result,
                                                const ArmMatrix<Type, rowa, cola>& a,
                                                const ArmMatrix<Type, rowb, colb>& b);

private:
    ArmMatrixType matrix{};  ///< CMSIS-DSP 矩阵实例
};

export template<std::size_t row, std::size_t col>
class ArmMatrix<double, row, col>;  // todo 待完成其他类型的特化

}  // namespace rmdev

#endif  // RMDEV_USE_CMSIS_DSP
