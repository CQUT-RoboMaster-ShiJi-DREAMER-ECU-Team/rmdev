/**
 * @file concepts.hpp
 * @author NaAlO2
 * @date 2025-06-07
 * @brief 
 */
 
#pragma once
#ifndef RMDEV_CONCEPTS_HPP
#define RMDEV_CONCEPTS_HPP

#include "etl/type_traits.h"

namespace rmdev {

/**
 * 可计算类型概念
 * @tparam Type 数据类型
 */
template<typename Type>
concept ArithmeticType = etl::is_arithmetic_v<Type>;

/**
 * 浮点类型概念
 * @tparam Type 数据类型
 */
template<typename Type>
concept FloatingPointType = etl::is_floating_point_v<Type>;

}  // namespace rmdev

#endif // !RMDEV_CONCEPTS_HPP
