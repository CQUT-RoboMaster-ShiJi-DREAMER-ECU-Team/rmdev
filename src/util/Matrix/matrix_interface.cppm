/**
 * @file matrix_interface.cppm
 * @module
 * @author NaAlO2
 * @date 2025-06-07
 * @brief
 */

module;

#include <cstddef>

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

protected:
    MatrixInterface() = default;
};

}  // namespace rmdev
