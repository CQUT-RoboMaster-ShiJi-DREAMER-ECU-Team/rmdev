/**
 * @file Matrix.cppm
 * @module rmdev.util.Matrix
 * @author 杜以成
 * @date 2025-06-02
 * @brief 矩阵运算
 */

module;

export module rmdev.util.Matrix;

export import rmdev.util.MatrixType;

#if RMDEV_USE_CMSIS_DSP
export import rmdev.util.ArmMatrix;
#endif
