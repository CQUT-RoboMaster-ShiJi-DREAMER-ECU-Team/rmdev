/**
 * @file basic.cppm
 * @module rmdev.util.math:basic
 * @author 杜以成
 * @date 2025-06-15
 * @brief 基本
 */

module;

#include <cmath>

#include "rmdev/concepts.hpp"

export module rmdev.util.math:basic;
import :const_value;

export namespace rmdev {

/**
 * 交换两个变量
 * @tparam Type: 待交换的变量的类型
 * @param a 第一个变量
 * @param b 第二个变量
 */
template<typename Type>
    requires ArithmeticType<Type>
void swap(Type& a, Type& b) noexcept
{
    Type tmp = a;
    a = b;
    b = tmp;
}

/**
 * 符号函数
 * @tparam Type: 数字类型（应当为整型数字）
 * @param n: 待判断符号的数字
 * @retval 0 n 等于零
 * @retval 1 n 大于零
 * @retval -1 n 小于零
 */
template<typename Type>
    requires ArithmeticType<Type>
constexpr Type sgn(const Type n)
{
    if (n > 0) {
        return 1;
    }
    if (n < 0) {
        return -1;
    }

    return 0;
}

/**
 * 符号函数 - @c float 类型特例
 * @param n: 待判断符号的数字
 * @retval 1.0f n 大于或等于零
 * @retval -1.0f n 小于零
 */
template<>
constexpr float sgn<float>(const float n)
{
    return ((n) >= 0.0f ? (1.0f) : (-1.0f));
}

/**
 * 符号函数 - @c double 类型特例
 * @param n: 待判断符号的数字
 * @retval 1.0 n 大于或等于零
 * @retval -1.0 n 小于零
 */
template<>
constexpr double sgn<double>(const double n)
{
    return ((n) >= 0.0 ? (1.0) : (-1.0));
}

/**
 * 浮点数等于比较
 * @tparam FloatType 具体的浮点类型
 * @param a 第一个数
 * @param b 第二个数
 * @param error 允许的误差（a、b 之差的绝对值小于这个值即认为相等）
 * @return a、b 是否相等
 */
template<typename FloatType>
    requires FloatingPointType<FloatType>
constexpr bool floatEqu(FloatType a,
                        FloatType b,
                        FloatType error = static_cast<FloatType>(float_equ_default_error_value))
{
    return std::abs(a - b) < error;
}

}  // namespace rmdev
