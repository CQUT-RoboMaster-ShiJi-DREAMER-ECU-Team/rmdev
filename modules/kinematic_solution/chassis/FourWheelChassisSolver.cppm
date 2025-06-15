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

/**
 * 底盘轮组的编号
 */
enum ChassisWheelsIndex : std::uint8_t {
    WHEEL_FL = 0U,  ///< 左前轮
    WHEEL_BL,       ///< 左后轮
    WHEEL_BR,       ///< 右后轮
    WHEEL_FR        ///< 右前轮
};

/**
 * 底盘轮组速度向量
 * @tparam Type 数据类型
 */
template<ArithmeticType Type>
using ChassisWheelsSpeed = std::array<Type, 4>;

/**
 * 底盘目标速度向量的索引
 */
enum ChassisTargetSpeedIndex : std::uint8_t {
    WHEEL_VX = 0,  ///< 前进速度
    WHEEL_VY,      ///< 侧向速度
    WHEEL_OMEGA    ///< 旋转角速度
};

/**
 * 底盘目标速度向量
 * @tparam Type 数据类型
 * @note 该向量包含三个分量：前进速度、侧向速度和旋转角速度，构成右手系
 */
template<ArithmeticType Type>
using ChassisTargetSpeed = std::array<Type, 3>;

/**
 * 四轮底盘解算接口（使用 CRTP 模式）
 * @tparam SolverType 子类类型
 */
template<class SolverType>
class FourWheelChassisSolver
{
private:
    using ScaleType_ = typename SolverType::ScaleType;  ///< 数据类型

public:
    /**
     * 解算底盘轮组速度
     * @param speed 底盘目标速度向量
     * @return 底盘轮组速度向量
     */
    auto solve(const ChassisTargetSpeed<ScaleType_> speed) const -> ChassisWheelsSpeed<ScaleType_>
    {
        return static_cast<const SolverType*>(this)->solve_impl(speed);
    }

protected:
    /**
     * 根据底盘长宽构造四轮底盘解算器
     * @param width 底盘宽度
     * @param length 底盘长度
     */
    constexpr FourWheelChassisSolver(const ScaleType_ width, const ScaleType_ length) noexcept
        : width_(width), length_(length)
    {
    }

    ~FourWheelChassisSolver() = default;

    const ScaleType_ width_;   ///< 底盘宽度
    const ScaleType_ length_;  ///< 底盘长度

    // 算法参考浙江大学的开源文档：
    // https://zju-helloworld.github.io/Wiki/%E7%BB%84%E4%BB%B6%E8%AF%B4%E6%98%8E%EF%BC%88%E6%97%A7%EF%BC%89/%E6%9C%BA%E5%99%A8%E4%BA%BA%E9%80%9A%E7%94%A8%E7%BB%84%E4%BB%B6/%E7%AE%97%E6%B3%95/%E5%BA%95%E7%9B%98%E9%80%86%E8%BF%90%E5%8A%A8%E5%AD%A6%E8%A7%A3%E7%AE%97/

    /* clang-format off */
    /// 各个轮组对旋转中心的位失的 x 方向的分量
    const std::array<ScaleType_, 4> rx{ length_ / ScaleType_(2),
                                       -length_ / ScaleType_(2),
                                       -length_ / ScaleType_(2),
                                        length_ / ScaleType_(2)};
    /// 各个轮组对旋转中心的位失的 y 方向的分量
    const std::array<ScaleType_, 4> ry{ width_ / ScaleType_(2),
                                        width_ / ScaleType_(2),
                                       -width_ / ScaleType_(2),
                                       -width_ / ScaleType_(2)};
    /* clang-format on */
};

}  // namespace rmdev
