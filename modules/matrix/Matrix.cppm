/**
 * @file Matrix.cppm
 * @module rmdev.util.Matrix
 * @author 杜以成
 * @date 2025-06-02
 * @brief 矩阵运算
 */

module;

#include "rmdev/concepts.hpp"

export module rmdev.util.Matrix;

export import rmdev.util.MatrixType;

#ifdef RMDEV_USE_CMSIS_DSP

export import rmdev.util.ArmMatrix;

export namespace rmdev {

template<typename Type, std::size_t row, std::size_t col>
    requires ArithmeticType<Type>
using Matrix = rmdev::ArmMatrix<Type, row, col>;

}  // namespace rmdev

#endif  // RMDEV_USE_CMSIS_DSP
