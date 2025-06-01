/**
 * @file math.cppm
 * @module rmdev.lib.math
 * @author 杜以成
 * @date 2025-05-09
 * @brief 数学相关函数
 */

module;

#include "etl/type_traits.h"
#include "etl/math.h"

export module rmdev.util.math;

export namespace rmdev {

/// 圆周率
constexpr auto PI = 3.14159265358979f;

/**
 * 可计算类型概念
 * @tparam Type 数据类型
 */
template<typename Type>
concept CalculatableType = requires { requires etl::is_arithmetic_v<Type>; };

/**
 * 交换两个变量
 * @tparam CalculatableType: 待交换的变量的类型
 * @param a 第一个变量
 * @param b 第二个变量
 */
template<typename CalculatableType>
inline void swap(CalculatableType& a, CalculatableType& b) noexcept
{
    CalculatableType tmp = a;
    a = b;
    b = tmp;
}

/**
 * 符号函数
 * @tparam CalculatableType: 数字类型（应当为整型数字）
 * @param n: 待判断符号的数字
 * @retval 0 n 等于零
 * @retval 1 n 大于零
 * @retval -1 n 小于零
 */
template<typename CalculatableType>
inline constexpr CalculatableType sgn(const CalculatableType n)
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
inline constexpr float sgn(const float n)
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
inline constexpr double sgn(const double n)
{
    return ((n) >= 0.0 ? (1.0) : (-1.0));
}

/**
 * 获得给定位数的无符号整型的最大值
 * @param bits: 位数
 * @return 最大值
 */
inline constexpr std::size_t bitUintMax(const std::size_t bits)
{
    return (1U << bits) - 1U;
}

/**
 * 计算给定十进制整数的位数
 * @param integer: 待计算的十进制整数
 * @return 这个十进制整数的位数
 */
inline constexpr std::size_t integerBits(std::size_t integer)
{
    std::size_t count = 0U;

    while (integer != 0U) {
        integer /= 10;
        ++count;
    }

    return count;
}

/**
 * 角度转弧度
 *
 * @param angle: 角度值
 * @return 弧度值
 */
inline constexpr float angleToRad(const float angle)
{
    return angle * PI / 180.0f;
}

/**
 * 角度转弧度
 *
 * @param angle: 角度值
 * @return 弧度值
 */
inline constexpr double angleToRad(const double angle)
{
    return angle * PI / 180.0;
}

/**
 * 弧度转角度
 *
 * @param rad: 弧度值
 * @return 角度值
 */
inline constexpr float radToAngle(const float rad)
{
    return rad * 180.0f / PI;
}

/**
 * 弧度转角度
 *
 * @param rad: 弧度值
 * @return 角度值
 */
inline constexpr double radToAngle(const double rad)
{
    return rad * 180.0 / PI;
}

/**
 * 限制值在给定范围内
 * @tparam CalculatableType 数据类型
 * @param value 待限制的值
 * @param min 最小值
 * @param max 最大值
 * @return 限制计算完成后的值
 */
template<typename CalculatableType>
inline constexpr CalculatableType limitMinMax(CalculatableType& value,
                                              const CalculatableType min,
                                              const CalculatableType max)
{
    value = etl::clamp(value, min, max);
    return value;
}

/**
 * 限制值在给定对称区间内
 * @tparam CalculatableType 数据类型
 * @param value 待限制的值
 * @param range 对称区间半径
 * @return 限制计算完成后的值
 */
template<typename CalculatableType>
inline constexpr CalculatableType limitInRange(CalculatableType& value, CalculatableType range)
{
    value = limitMinMax(value, -range, range);
    return value;
}

}  // namespace rmdev
