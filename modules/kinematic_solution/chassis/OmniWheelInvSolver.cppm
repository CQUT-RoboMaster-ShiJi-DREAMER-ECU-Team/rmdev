/**
 * @file OmniWheelInvSolver.cppm
 * @module rmdev.kinematic_solution.chassis.OmniWheelInvSolver
 * @author 杜以成
 * @date 2025-06-14
 * @brief 全向轮逆运动学解算
 */

module;

#include <cmath>

#include "rmdev/concepts.hpp"

export module rmdev.kinematic_solution.chassis.OmniWheelInvSolver;

import rmdev.kinematic_solution.chassis.FourWheelChassisSolver;
export import rmdev.util.Matrix;
import rmdev.util.math;

export namespace rmdev {

/**
 * 全向轮逆运动学解算类
 * @tparam Type 数据类型
 * @tparam wheel_num 底盘轮组数量（默认4个）
 */
template<ArithmeticType Type, std::size_t wheel_num = 4>
class OmniWheelInvSolver
{
public:
    using ScaleType = Type;
};

/**
 * 全向轮逆运动学解算类 - 四轮轮组特化
 * @tparam Type 数据类型
 */
template<ArithmeticType Type>
class OmniWheelInvSolver<Type, 4> : public FourWheelChassisSolver<OmniWheelInvSolver<Type, 4>, Type>
{
public:
    /**
     * 数据类型
     */
    using ScaleType = Type;

    OmniWheelInvSolver() = delete;

    friend class FourWheelChassisSolver<OmniWheelInvSolver<Type, 4>, Type>;

    /**
     * 通过底盘宽度和长度构造全向轮逆运动学解算器
     * @param width 底盘宽度
     * @param length 底盘长度
     */
    constexpr OmniWheelInvSolver(const ScaleType width, const ScaleType length) noexcept
        : FourWheelChassisSolver<OmniWheelInvSolver, ScaleType>(width, length)
    {
    }

    /**
     * 构造正方形底盘的全向轮逆运动学解算器
     * @param square_length 底盘边长
     */
    explicit constexpr OmniWheelInvSolver(const ScaleType square_length) noexcept
        : OmniWheelInvSolver(square_length, square_length)
    {
    }

private:
    auto solve_impl(const ChassisTargetSpeed<Type>& speed) const -> ChassisWheelsSpeed<ScaleType>
    {
        const Matrix<ScaleType, 3, 1> target_speed{speed[WHEEL_VX], speed[WHEEL_VY], speed[WHEEL_OMEGA]};

        Matrix<ScaleType, 4, 1> wheel_speeds;

        mul(wheel_speeds, traits_matrix, target_speed);
        mul(wheel_speeds, ScaleType(CONST_SQRT1_2), wheel_speeds);

        return {wheel_speeds(1, 1), wheel_speeds(2, 1), wheel_speeds(3, 1), wheel_speeds(4, 1)};
    }

    /* clang-format off */
    const Matrix<ScaleType, 4, 3> traits_matrix{{1, -1, -this->rx[1] - this->ry[1]},
                                                {1,  1,  this->rx[2] - this->ry[2]},
                                                {1, -1, -this->rx[3] - this->ry[3]},
                                                {1,  1,  this->rx[4] - this->ry[4]}};
    /* clang-format on */
};

}  // namespace rmdev
