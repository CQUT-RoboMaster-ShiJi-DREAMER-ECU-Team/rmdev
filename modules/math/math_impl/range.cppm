/**
 * @file range.cppm
 * @module rmdev.util.math:range
 * @author 杜以成
 * @date 2025-06-15
 * @brief 范围相关的
 */

module;

#include <algorithm>

#include "rmdev/concepts.hpp"

export module rmdev.util.math:range;

export namespace rmdev {

/**
 * 限制值在给定范围内
 * @tparam Type 数据类型
 * @param value 待限制的值
 * @param min 最小值
 * @param max 最大值
 * @return 限制计算完成后的值
 */
template<typename Type>
    requires ArithmeticType<Type>
constexpr Type limitMinMax(Type& value, const Type min, const Type max)
{
    value = std::clamp(value, min, max);
    return value;
}

/**
 * 限制值在给定对称区间内
 * @tparam Type 数据类型
 * @param value 待限制的值
 * @param range 对称区间半径
 * @return 限制计算完成后的值
 */
template<typename Type>
    requires ArithmeticType<Type>
constexpr Type limitInRange(Type& value, Type range)
{
    value = limitMinMax(value, -range, range);
    return value;
}

}  // namespace rmdev
