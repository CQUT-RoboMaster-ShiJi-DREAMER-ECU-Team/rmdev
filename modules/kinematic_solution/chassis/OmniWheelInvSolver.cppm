/**
 * @file OmniWheelInvSolver.cppm
 * @module rmdev.kinematic_solution.chassis.OmniWheelInvSolver
 * @author 杜以成
 * @date 2025-06-14
 * @brief 全向轮逆运动学解算
 */

module;

#include "rmdev/concepts.hpp"

export module rmdev.kinematic_solution.chassis.OmniWheelInvSolver;

import rmdev.util.Matrix;

namespace rmdev {

template<typename Type, std::size_t wheel_num>
    requires ArithmeticType<Type>
class OmniWheelInvSolver;

template<typename Type>
    requires ArithmeticType<Type>
class OmniWheelInvSolver<Type, 4>
{
public:
private:
};

}  // namespace rmdev
