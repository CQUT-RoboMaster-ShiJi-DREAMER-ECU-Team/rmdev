/**
 * @file Matrix_def.cppm
 * @module rmdev.Matrix:def
 * @author 杜以成
 * @date 2025-06-02
 * @brief 矩阵计算定义
 */

module;

#include <cstring>
#include "etl/type_traits.h"
#include "arm_math.h"

export module rmdev.Matrix:def;

export namespace rmdev {

template<typename Type, std::size_t row, std::size_t col>
class Matrix;

template<std::size_t row, std::size_t col>
class Matrix<float, row, col>
{
public:
    static_assert(etl::is_same_v<float, float32_t>);

    /**
     *
     */
    Matrix();

    Matrix(const Matrix& other);

    Matrix& operator=(const Matrix& other);

    static Matrix& add(Matrix& result, const Matrix& a, const Matrix& b);

    static Matrix& subtract(Matrix& result, const Matrix& a, const Matrix& b);

    template<std::size_t rowa, std::size_t cola, std::size_t rowb, std::size_t colb>
    static auto multiply(Matrix<float, rowa, colb>& result,
                         const Matrix<float, rowa, cola>& a,
                         const Matrix<float, rowb, colb>& b)
        -> etl::enable_if_t<cola == rowb, Matrix<float, rowa, colb>&>;

    static Matrix& multiply(Matrix& result, const Matrix& a, float scalar);

    static Matrix& multiply(Matrix& result, float scalar, const Matrix& a);

    static Matrix<float, col, row>& transpose(Matrix<float, col, row>& result, const Matrix<float, row, col>& a);

    static auto inverse(Matrix& result, const Matrix& a) -> etl::enable_if_t<row == col, Matrix&>;

    static auto determinant(Matrix& a) -> etl::enable_if_t<row == col, float>;

private:
    arm_matrix_instance_f32 matrix{};
    float32_t data[row * col]{};
};

template<std::size_t row, std::size_t col>
class Matrix<float64_t, row, col>
{
public:
    static_assert(etl::is_same_v<double, float64_t>);
    /**
     *
     */
    Matrix()
    {
        arm_mat_init_f64(&matrix, row, col, data);
    }

private:
    arm_matrix_instance_f64 matrix{};
    float64_t data[row * col]{};
};

}  // namespace rmdev
