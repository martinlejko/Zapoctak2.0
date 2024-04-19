//
// Created by Martin Lejko on 31/01/2024.
//

#include "datatypes.h"
#include <cmath>
#include <iostream>

int Vec2::crossProduct(const Vec2 &v) const {
    return x * v.y - y * v.x;
}

Vec3 Vec3::crossProduct(const Vec3& v) const {
    return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

void Vec3::normalize() {
    float length = std::sqrt(x * x + y * y + z * z);
    if (length != 0) {
        x /= length;
        y /= length;
        z /= length;
    }
}

float Vec3::dotProduct(const Vec3& v) const {
    return x * v.x + y * v.y + z * v.z;
}

template<typename T>
Matrix<T>::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols), data(rows, std::vector<T>(cols)) {}

template<typename T>
size_t Matrix<T>::numRows() const { return rows; }

template<typename T>
size_t Matrix<T>::numCols() const { return cols; }

template<typename T>
T& Matrix<T>::operator()(size_t i, size_t j) {
    if (i >= rows || j >= cols)
        throw std::out_of_range("Index out of range");
    return data[i][j];
}

template<typename T>
const T& Matrix<T>::operator()(size_t i, size_t j) const {
    if (i >= rows || j >= cols)
        throw std::out_of_range("Index out of range");
    return data[i][j];
}

template<typename T>
Matrix<T> Matrix<T>::identity(size_t size) {
    Matrix<T> identityMatrix(size, size);
    for (size_t i = 0; i < size; ++i) {
        identityMatrix(i, i) = static_cast<T>(1);
    }
    return identityMatrix;
}

// Multiplication
template<typename T>
Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    if (lhs.numCols() != rhs.numRows())
        throw std::invalid_argument("Matrix dimensions mismatch");

    size_t m = lhs.numRows();
    size_t n = lhs.numCols();
    size_t p = rhs.numCols();

    Matrix<T> result(m, p);

    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < p; ++j) {
            T sum = 0;
            for (size_t k = 0; k < n; ++k) {
                sum += lhs(i, k) * rhs(k, j);
            }
            result(i, j) = sum;
        }
    }

    return result;
}

template<typename T>
Matrix<T> Matrix<T>::transpose() const {
    Matrix<T> result(cols, rows);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result(j, i) = data[i][j];
        }
    }
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::inverse() const {
    if (rows != cols)
        throw std::logic_error("Matrix must be square for inversion");

    size_t n = rows;
    Matrix<T> identityMat = Matrix<T>::identity(n);
    Matrix<T> augmentedMat(n, 2 * n);

    // Copy the original matrix and identity matrix to the augmented matrix
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            augmentedMat(i, j) = data[i][j];
            augmentedMat(i, j + n) = identityMat(i, j);
        }
    }

    // Perform Gauss-Jordan elimination to obtain the inverse
    for (size_t i = 0; i < n; ++i) {
        // Pivot for column i
        T pivot = augmentedMat(i, i);

        // Divide the row by the pivot to make the pivot 1
        for (size_t j = 0; j < 2 * n; ++j) {
            augmentedMat(i, j) /= pivot;
        }

        // Make other rows 0 in column i
        for (size_t k = 0; k < n; ++k) {
            if (k != i) {
                T factor = augmentedMat(k, i);
                for (size_t j = 0; j < 2 * n; ++j) {
                    augmentedMat(k, j) -= factor * augmentedMat(i, j);
                }
            }
        }
    }

    // Extract the inverse matrix from the augmented matrix
    Matrix<T> inverseMat(n, n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            inverseMat(i, j) = augmentedMat(i, j + n);
        }
    }

    return inverseMat;
}

template<typename T>
void Matrix<T>::display() const {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            std::cout << data[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

template<typename T>
std::vector<T> Matrix<T>::toVector() const {
    std::vector<T> vec;
    vec.reserve(rows * cols);
    for (const auto& row : data) {
        for (const auto& val : row) {
            vec.push_back(val);
        }
    }
    return vec;
}

template<typename T>
Matrix<T> Matrix<T>::fromVector(const std::vector<T>& vec, size_t rows, size_t cols) {
    if (vec.size() != rows * cols) {
        throw std::invalid_argument("Vector size does not match matrix dimensions");
    }

    Matrix<T> mat(rows, cols);

    size_t index = 0;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            mat(i, j) = vec[index++];
        }
    }

    return mat;
}

template<typename T>
Matrix<T> Matrix<T>::viewport(int x, int y, int w, int h, int depth) {
    Matrix<T> m = Matrix<T>::identity(4);
    m(0, 3) = x + w / 2.f;
    m(1, 3) = y + h / 2.f;
    m(2, 3) = depth / 2.f;

    m(0, 0) = w / 2.f;
    m(1, 1) = h / 2.f;
    m(2, 2) = depth / 2.f;
    return m;
}