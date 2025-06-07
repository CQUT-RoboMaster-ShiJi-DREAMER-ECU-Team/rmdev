/**
 * @file matrix_interface.cppm
 * @module rmdev.Matrix:interface
 * @author 杜以成
 * @date 2025-06-07
 * @brief 矩阵接口
 */

module;

#include <cstddef>

#include "rmdev/concepts.hpp"

export module rmdev.Matrix:interface;

export namespace rmdev {

template<template<typename Type, std::size_t row, std::size_t col> class Matrix>
class MatrixInterface
{
public:
    template<typename Type, std::size_t row, std::size_t col>
    Matrix<Type, row, col>& operator+(const Matrix<Type, row, col>& other)
    {
        return (static_cast<Matrix<Type, row, col>*>(this))->opadd_impl(other);
    }

    template<typename Type, std::size_t row, std::size_t col>
    Matrix<Type, row, col>& operator-(const Matrix<Type, row, col>& other)
    {
        return (static_cast<Matrix<Type, row, col>*>(this))->opsub_impl(other);
    }

    template<typename Type, std::size_t row, std::size_t col>
    Matrix<Type, row, col>& operator*(const Type other)
    {
        return (static_cast<Matrix<Type, row, col>*>(this))->opmul_impl(other);
    }

    template<typename Type, std::size_t row1, std::size_t col1, std::size_t row2, std::size_t col2>
        requires MatrixCouldMultiplied<row1, col1, row2, col2>
    friend Matrix<Type, row1, col2>& operator*(const Matrix<Type, row1, col1>& mat1,
                                               const Matrix<Type, row2, col2>& mat2);

protected:
    MatrixInterface() = default;
};

}  // namespace rmdev
