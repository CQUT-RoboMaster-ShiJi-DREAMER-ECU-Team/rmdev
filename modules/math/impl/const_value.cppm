/**
 * @file const_value.cppm
 * @module rmdev.util.math:const_value
 * @author 杜以成
 * @date 2025-06-15
 * @brief 数学常用常量
 */

module;

#ifndef RMDEV_FLOAT_EQU_DEFAULT_ERROR_VALUE

/// 默认的浮点数等于比较误差值（当两个浮点数的绝对值之差小于这个值时，认为二者相等）
/// @note 这个值可以通过编译参数定义宏 RMDEV_FLOAT_EQU_DEFAULT_ERROR_VALUE 来修改。
///       如果没有定义，则使用默认值 0.001f
#define RMDEV_FLOAT_EQU_DEFAULT_ERROR_VALUE 0.001f

#endif

export module rmdev.util.math:const_value;

export namespace rmdev {

/// rmdev 中，默认的浮点数等于比较误差值
constexpr auto float_equ_default_error_value = RMDEV_FLOAT_EQU_DEFAULT_ERROR_VALUE;

constexpr auto CONST_E = 2.71828182845904523536;         ///< e
constexpr auto CONST_LOG2E = 1.44269504088896340736;     ///< log2(e)
constexpr auto CONST_LOG10E = 0.434294481903251827651;   ///< log10(e)
constexpr auto CONST_LN2 = 0.693147180559945309417;      ///< ln(2)
constexpr auto CONST_LN10 = 2.30258509299404568402;      ///< ln(10)
constexpr auto CONST_PI = 3.14159265358979323846;        ///< pi
constexpr auto CONST_PI_2 = 1.57079632679489661923;      ///< pi/2
constexpr auto CONST_PI_4 = 0.785398163397448309616;     ///< pi/4
constexpr auto CONST_1_PI = 0.318309886183790671538;     ///< 1/pi
constexpr auto CONST_2_PI = 0.636619772367581343076;     ///< 2/pi
constexpr auto CONST_2_SQRTPI = 1.12837916709551257390;  ///< 2/sqrt(pi)
constexpr auto CONST_SQRT2 = 1.41421356237309504880;     ///< sqrt(2)
constexpr auto CONST_SQRT1_2 = 0.707106781186547524401;  ///< 1/sqrt(2)

}  // namespace rmdev
