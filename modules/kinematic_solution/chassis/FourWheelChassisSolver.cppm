/**
 * @file FourWheelChassisSolver.cppm
 * @module rmdev.kinematic_solution.chassis.FourWheelChassisSolver
 * @author 杜以成
 * @date 2025-06-15
 * @brief 四轮底盘解算接口
 */

module;

#include <cstdint>
#include <array>

#include "rmdev/concepts.hpp"

export module rmdev.kinematic_solution.chassis.FourWheelChassisSolver;

export namespace rmdev {

enum WheelIndex : std::uint8_t {
    WHEEL_FL = 0U,
    WHEEL_BL,
    WHEEL_BR,
    WHEEL_FR
};

template<ArithmeticType Type>
using WheelSpeed = std::array<Type, 4>;

enum WheelSpeedIndex : std::uint8_t {
    WHEEL_VX = 0,
    WHEEL_VY,
    WHEEL_OMEGA
};

template<ArithmeticType Type>
using TargetSpeed = std::array<Type, 3>;

template<class SolverType>
class FourWheelChassisSolver
{
public:
    auto solve(const TargetSpeed<typename SolverType::ScaleType> speed) const -> WheelSpeed<typename SolverType::ScaleType>
    {
        return static_cast<const SolverType*>(this)->solve_impl(speed);
    }

protected:
    FourWheelChassisSolver() = default;
    ~FourWheelChassisSolver() = default;
};

}  // namespace rmdev
