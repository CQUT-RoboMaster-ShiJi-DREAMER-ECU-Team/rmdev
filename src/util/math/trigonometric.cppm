/**
 * @file trigonometric.cppm
 * @module rmdev.util.math:trigonometric
 * @author 杜以成
 * @date 2025-06-15
 * @brief 三角学相关函数
 */

module;

#include <cmath>

#ifdef RMDEV_USE_CMSIS_DSP
#include "arm_math.h"
#endif

#include "rmdev/concepts.hpp"

export module rmdev.util.math:trigonometric;
import :const_value;

export namespace rmdev {

/**
 * 角度转弧度
 * @tparam Type: 数据类型
 * @param angle: 角度值
 * @return 弧度值
 */
template<ArithmeticType Type>
constexpr float angleToRad(const Type angle)
{
    return angle * Type(CONST_PI) / Type(180);
}

/**
 * 弧度转角度
 * @tparam Type: 数据类型
 * @param rad: 弧度值
 * @return 角度值
 */
template<ArithmeticType Type>
constexpr float radToAngle(const Type rad)
{
    return rad * Type(180) / Type(CONST_PI);
}

/**
 * 编译期计算正弦
 * @tparam x 弧度
 */
template<auto x>
struct sin_constval {
    static_assert(std::is_arithmetic_v<decltype(x)>, "x must be an arithmetic type");
    static constexpr auto value = std::sin(x);  ///< 计算结果
};

/**
 * 编译期计算正弦 - 直接获取值的简化版本
 * @tparam x 弧度
 */
template<auto x>
inline auto sin_v = sin_constval<x>::value;

/**
 * 计算正弦值
 * @tparam Type 数据类型
 * @param x 弧度
 * @return 计算结果
 */
template<ArithmeticType Type>
Type sin(const Type x)
{
#ifdef RMDEV_USE_CMSIS_DSP
    if constexpr (std::is_same_v<Type, float32_t>) {
        return arm_sin_f32(x);
    }
    else if constexpr (std::is_same_v<Type, q31_t>) {
        return arm_sin_q31(x);
    }
    else if constexpr (std::is_same_v<Type, q15_t>) {
        return arm_sin_q15(x);
    }
    else {
        return std::sin(x);
    }
#else
    return std::sin(x);
#endif
}

/**
 * 编译期计算余弦
 * @tparam x 弧度
 */
template<auto x>
struct cos_constval {
    static_assert(std::is_arithmetic_v<decltype(x)>, "x must be an arithmetic type");
    static constexpr auto value = std::cos(x);
};

/**
 * 编译期计算余弦 - 直接获取值的简化版本
 * @tparam x 弧度
 */
template<auto x>
inline auto cos_v = cos_constval<x>::value;

/**
 * 计算余弦值
 * @tparam Type 数据类型
 * @param x 弧度
 * @return 计算结果
 */
template<ArithmeticType Type>
Type cos(const Type x)
{
#ifdef RMDEV_USE_CMSIS_DSP
    if constexpr (std::is_same_v<Type, float32_t>) {
        return arm_cos_f32(x);
    }
    else if constexpr (std::is_same_v<Type, q31_t>) {
        return arm_cos_q31(x);
    }
    else if constexpr (std::is_same_v<Type, q15_t>) {
        return arm_cos_q15(x);
    }
    else {
        return std::cos(x);
    }
#else
    return std::cos(x);
#endif
}

/**
 * 计算正弦和余弦值
 * @tparam Type 数据类型
 * @param x 弧度制
 * @param[out] sin_out 正弦计算结果
 * @param[out] cos_out 余弦计算结果
 */
template<ArithmeticType Type>
void sin_cos(const Type x, Type& sin_out, Type& cos_out)
{
#ifdef RMDEV_USE_CMSIS_DSP
    if constexpr (std::is_same_v<Type, float32_t>) {
        arm_sin_cos_f32(x, &sin_out, &cos_out);
    }
    else if constexpr (std::is_same_v<Type, q31_t>) {
        arm_sin_cos_q31(x, &sin_out, &cos_out);
    }
    else {
        sin_out = std::sin(x);
        cos_out = std::cos(x);
    }
#else
    sin_out = std::sin(x);
    cos_out = std::cos(x);
#endif
}

}  // namespace rmdev
