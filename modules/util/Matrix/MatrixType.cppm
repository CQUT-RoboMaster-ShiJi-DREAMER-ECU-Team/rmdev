/**
 * @file MatrixType.cppm
 * @module rmdev.util.MatrixType
 * @author 杜以成
 * @date 2025-06-12
 * @brief 特殊矩阵类型
 */

module;

#include <cstdint>

export module rmdev.util.MatrixType;

export namespace rmdev {

/**
 * 特殊矩阵类型
 */
enum class MatrixType : uint8_t {
    Normal = 0U,  ///< 普通矩阵
    E,            ///< 单位矩阵（必须是方阵才有意义，否则无效）
    One,          ///< 每个元素全是 1 的矩阵
    Zero          ///< 每个元素全是 0 的矩阵
};  // todo 可以考虑实现：上、下三角矩阵

}  // namespace rmdev
