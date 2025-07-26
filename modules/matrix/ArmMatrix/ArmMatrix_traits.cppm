/**
 * @file ArmMatrix_traits.cppm
 * @module rmdev.util.ArmMatrix:traits
 * @author 杜以成
 * @date 2025-06-13
 * @brief 类型特征
 */

module;

#include <type_traits>

#include "arm_math.h"

export module rmdev.util.ArmMatrix:traits;

namespace rmdev {

/**
 * ArmMatrix 类型特征 - 用于根据数据类型自动选择相应接口
 * @tparam ScaleType 数据类型
 */
template<typename ScaleType>
struct ArmMatrixTraits;

// todo 待实现其他类型的特化（不过其他类型的用得少，不写也不是不行）））

/**
 * ArmMatrix 类型特征 - float 类型特化
 */
template<>
struct ArmMatrixTraits<float> {
    static_assert(std::is_same_v<float, float32_t>);

    using DataType = float;                             ///< 数据类型
    using ArmMatrixInstance = arm_matrix_instance_f32;  ///< arm 矩阵实例类型

    // 由于 CMSIS 提供的初始化函数不是 constexpr 的，因此此处不调用 CMSIS 提供的初始化函数，而是自己实现
    // static auto init(ArmMatrixInstance* S, const uint16_t nRows, const uint16_t nColumns, DataType* pData)
    // {
    //     return arm_mat_init_f32(S, nRows, nColumns, pData);
    // }

    constexpr static auto init(ArmMatrixInstance* S, const uint16_t nRows, const uint16_t nColumns, DataType* pData)
    {
        S->numRows = nRows;
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

    static auto inverse(const ArmMatrixInstance* src, ArmMatrixInstance* dst)
    {
        return arm_mat_inverse_f32(src, dst);
    }

    static auto trans(const ArmMatrixInstance* pSrc, ArmMatrixInstance* pDst)
    {
        return arm_mat_trans_f32(pSrc, pDst);
    }
};

/**
 * ArmMatrix 类型特征 - double 类型特化
 */
template<>
struct ArmMatrixTraits<double> {
    static_assert(std::is_same_v<double, float64_t>);

    using DataType = double;
    using ArmMatrixInstance = arm_matrix_instance_f64;

    constexpr static auto init(ArmMatrixInstance* S, const uint16_t nRows, const uint16_t nColumns, DataType* pData)
    {
        S->numRows = nRows;
        S->numCols = nColumns;
        S->pData = pData;
    }

    // 目前适用于 double 类型的只有求逆的函数，其他的都没有……

    // static auto add(const ArmMatrixInstance* pSrcA, const ArmMatrixInstance* pSrcB, ArmMatrixInstance* pDst)
    // {
    //     return arm_mat_add_f64(pSrcA, pSrcB, pDst);
    // }
    //
    // static auto sub(const ArmMatrixInstance* pSrcA, const ArmMatrixInstance* pSrcB, ArmMatrixInstance* pDst)
    // {
    //     return arm_mat_sub_f64(pSrcA, pSrcB, pDst);
    // }
    //
    // static auto scale(const ArmMatrixInstance* pSrc, const DataType scale, ArmMatrixInstance* pDst)
    // {
    //     return arm_mat_scale_f64(pSrc, scale, pDst);
    // }
    //
    // static auto mult(const ArmMatrixInstance* pSrcA, const ArmMatrixInstance* pSrcB, ArmMatrixInstance* pDst)
    // {
    //     return arm_mat_mult_f64(pSrcA, pSrcB, pDst);
    // }

    static auto inverse(const ArmMatrixInstance* src, ArmMatrixInstance* dst)
    {
        return arm_mat_inverse_f64(src, dst);
    }

    // static auto trans(const ArmMatrixInstance* pSrc, ArmMatrixInstance* pDst)
    // {
    //     return arm_mat_trans_f64(pSrc, pDst);
    // }
};

}  // namespace rmdev
