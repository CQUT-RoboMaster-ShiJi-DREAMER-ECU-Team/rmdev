/**
 * @file OmniWheelInvSolver.cppm
 * @module rmdev.kinematic_solution.chassis.OmniWheelInvSolver
 * @author 杜以成
 * @date 2025-06-14
 * @brief 全向轮逆运动学解算
 */

module;

#include <cmath>
#include <array>

#include "rmdev/concepts.hpp"

export module rmdev.kinematic_solution.chassis.OmniWheelInvSolver;

import rmdev.kinematic_solution.chassis.FourWheelChassisSolver;
import rmdev.util.Matrix;
import rmdev.util.math;

export namespace rmdev {

template<ArithmeticType Type, std::size_t wheel_num = 4>
class OmniWheelInvSolver;

template<ArithmeticType Type>
class OmniWheelInvSolver<Type, 4> : public FourWheelChassisSolver<OmniWheelInvSolver<Type, 4>>
{
public:
    using ScaleType = Type;

    OmniWheelInvSolver() = delete;

    constexpr OmniWheelInvSolver(const ScaleType width, const ScaleType length) noexcept
        : width_(width), length_(length)
    {
    }

    explicit constexpr OmniWheelInvSolver(const ScaleType square_length) noexcept
        : OmniWheelInvSolver(square_length, square_length)
    {
    }

    auto solve_impl(const TargetSpeed<Type> speed) const -> WheelSpeed<ScaleType>
    {
        const Matrix<ScaleType, 3, 1> target_speed{speed[WHEEL_VX], speed[WHEEL_VY], speed[WHEEL_OMEGA]};

        Matrix<ScaleType, 4, 1> wheel_speeds;

        mul(wheel_speeds, traits_matrix, target_speed);
        mul(wheel_speeds, CONST_SQRT1_2, wheel_speeds);

        return {wheel_speeds(1, 1), wheel_speeds(2, 1), wheel_speeds(3, 1), wheel_speeds(4, 1)};
    }

private:
    const ScaleType width_;
    const ScaleType length_;

    /* clang-format off */
    const std::array<ScaleType, 4> rx{ length_ / ScaleType(2),
                                      -length_ / ScaleType(2),
                                      -length_ / ScaleType(2),
                                       length_ / ScaleType(2)};
    const std::array<ScaleType, 4> ry{ width_ / ScaleType(2),
                                       width_ / ScaleType(2),
                                      -width_ / ScaleType(2),
                                      -width_ / ScaleType(2)};

    const Matrix<ScaleType, 4, 3> traits_matrix{{1, -1, -rx[1] - ry[1]},
                                                {1,  1,  rx[2] - ry[2]},
                                                {1, -1, -rx[3] - ry[3]},
                                                {1,  1,  rx[4] - ry[4]}};
    /* clang-format on */
};

}  // namespace rmdev
