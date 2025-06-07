/**
 * @file Matrix.cppm
 * @module rmdev.Matrix
 * @author 杜以成
 * @date 2025-06-02
 * @brief 矩阵运算
 */

module;

export module rmdev.Matrix;

export import :interface;

#if RMDEV_USE_CMSIS_DSP
export import :CMSIS_DSP;
#endif
