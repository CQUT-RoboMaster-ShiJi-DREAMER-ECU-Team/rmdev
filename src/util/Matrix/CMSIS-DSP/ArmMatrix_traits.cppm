/**
 * @file ArmMatrix_traits.cppm
 * @module rmdev.util.ArmMatrix:traits
 * @author 杜以成
 * @date 2025-06-13
 * @brief 类型特征
 */

#if RMDEV_USE_CMSIS_DSP

module;

#include <type_traits>

#include "arm_math.h"

export module rmdev.util.ArmMatrix:traits;

namespace rmdev {

template<typename ScaleType>
struct ArmMatrixTraits;

template<>
struct ArmMatrixTraits<float> {
    static_assert(std::is_same_v<float, float32_t>);

    using DataType = float;
    using ArmMatrixInstance = arm_matrix_instance_f32;

    constexpr static auto init(ArmMatrixInstance* S, const uint16_t nRows, const uint16_t nColumns, DataType* pData)
    {
        // return arm_mat_init_f32(S, nRows, nColumns, pData);

        S->numRows = nRows;  // todo 待测试
        S->numCols = nColumns;
        S->pData = pData;
    }

    static auto add(const ArmMatrixInstance* pSrcA, const ArmMatrixInstance* pSrcB, ArmMatrixInstance* pDst)
    {
        return arm_mat_add_f32(pSrcA, pSrcB, pDst);
    }

    static auto sub(const ArmMatrixInstance* pSrcA, const ArmMatrixInstance* pSrcB, ArmMatrixInstance* pDst)
    {
        return arm_mat_sub_f32(pSrcA, pSrcB, pDst);
    }

    static auto scale(const ArmMatrixInstance* pSrc, const DataType scale, ArmMatrixInstance* pDst)
    {
        return arm_mat_scale_f32(pSrc, scale, pDst);
    }

    static auto mult(const ArmMatrixInstance* pSrcA, const ArmMatrixInstance* pSrcB, ArmMatrixInstance* pDst)
    {
        return arm_mat_mult_f32(pSrcA, pSrcB, pDst);
    }

    static auto inverse(const arm_matrix_instance_f32* src, arm_matrix_instance_f32* dst)
    {
        return arm_mat_inverse_f32(src, dst);
    }

    static auto trans(const arm_matrix_instance_f32* pSrc, arm_matrix_instance_f32* pDst)
    {
        return arm_mat_trans_f32(pSrc, pDst);
    }
};

template<>
struct ArmMatrixTraits<double> {
    static_assert(std::is_same_v<double, float64_t>);

    using DataType = double;
    using ArmMatrixInstance = arm_matrix_instance_f64;
};

}  // namespace rmdev

#endif  // RMDEV_USE_CMSIS_DSP
