//
// Created by Martin Lejko on 31/01/2024.
//

#ifndef ZAPOCTAK2_0_DATATYPES_H
#define ZAPOCTAK2_0_DATATYPES_H

#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <iostream>

struct Vertex {
    float x;
    float y;
    float z;
};

struct VertexNormal {
    float x;
    float y;
    float z;
};

struct UVVector {
    float x;
    float y;
};

struct FaceElement {
    int vertexIndex;
    int textureIndex;
    int normalIndex;
};

struct WaveFrontData {
    std::unordered_map<int, Vertex> vertices;
    std::unordered_map<int, VertexNormal> vertexNormals;
    std::unordered_map<int, UVVector> uvVectors;
    std::unordered_map<int, std::vector<FaceElement>> faces;

};

struct Vec2 {
    int x;
    int y;

    Vec2(int xVal = 0, int yVal = 0) : x(xVal), y(yVal) {}
    Vec2(const Vertex& v1, const Vertex& v2) : x(v2.x - v1.x), y(v2.y - v1.y) {}

    int crossProduct(const Vec2& v) const;
};

struct Vec3 {
    float x;
    float y;
    float z;

    Vec3(float xVal = 0, float yVal = 0, float zVal = 0) : x(xVal), y(yVal), z(zVal) {}
    Vec3(const Vertex& v1, const Vertex& v2) : x(v2.x - v1.x), y(v2.y - v1.y), z(v2.z - v1.z) {}

    Vec3 crossProduct(const Vec3& v) const;
    float dotProduct(const Vec3& v) const;
    void normalize();
    float norm () const { return std::sqrt(x*x+y*y+z*z); }

    Vec3 operator-(const Vec3& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }

    float& operator[](int index) {
        if (index == 0)
            return x;
        else if (index == 1)
            return y;
        else if (index == 2)
            return z;
        else
            throw std::out_of_range("Index out of range for Vec3");
    }

    const float& operator[](int index) const {
        if (index == 0)
            return x;
        else if (index == 1)
            return y;
        else if (index == 2)
            return z;
        else
            throw std::out_of_range("Index out of range for Vec3");
    }
};

struct TriangleData {
    Vertex v1;
    Vertex v2;
    Vertex v3;
};

struct UVTriangleData {
    UVVector uv1;
    UVVector uv2;
    UVVector uv3;
};

template<typename T>
class Matrix {
private:
    std::vector<std::vector<T>> data;
    size_t rows;
    size_t cols;

public:
    Matrix(size_t rows, size_t cols);

    size_t numRows() const;
    size_t numCols() const;

    T& operator()(size_t i, size_t j);
    const T& operator()(size_t i, size_t j) const;
    std::vector<T>& operator[](size_t i);
    const std::vector<T>& operator[](size_t i) const;

    static Matrix<T> identity(size_t size);
    template<class U>
    friend Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs);
    Matrix<T> transpose() const;
    Matrix<T> inverse() const;
    void display() const;
    static Matrix<T> viewport(int x, int y, int w, int h, int depth = 255);
    static Matrix<T> lookat(const Vec3 &eye, const Vec3 &center, const Vec3 &up);
};

static Matrix<float> toMatrix(const Vertex& v);

template<typename T>
Matrix<T>::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols), data(rows, std::vector<T>(cols)) {}

template<typename T>
std::vector<T>& Matrix<T>::operator[](size_t i) {
    if (i >= rows)
        throw std::out_of_range("Index out of range");
    return data[i];
}

template<typename T>
const std::vector<T>& Matrix<T>::operator[](size_t i) const {
    if (i >= rows)
        throw std::out_of_range("Index out of range");
    return data[i];
}

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

template<typename T>
Matrix<T> Matrix<T>::lookat(const Vec3 &eye, const Vec3 &center, const Vec3 &up) {
    Vec3 z = (eye - center);
    z.normalize();
    Vec3 x = up.crossProduct(z);
    x.normalize();
    Vec3 y = z.crossProduct(x);
    y.normalize();

    Matrix<T> Minv = Matrix<T>::identity(4);
    Matrix<T> Tr = Matrix<T>::identity(4);
    for (int i = 0; i < 3; i++) {
        Minv(0, i) = x[i];
        Minv(1, i) = y[i];
        Minv(2, i) = z[i];
        Tr(3, i) = -center[i];
    }
    return Minv * Tr;
}



template<typename T>
Matrix<T> multiply(const Matrix<T>& lhs, const Matrix<T>& mid, const Matrix<T>& rhs) {
    if (lhs.numCols() != mid.numRows() || mid.numCols() != rhs.numRows())
        throw std::invalid_argument("Matrix dimensions mismatch");

    size_t m = lhs.numRows();
    size_t n = mid.numRows();
    size_t p = rhs.numCols();

    Matrix<T> result(m, p);

    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < p; ++j) {
            T sum = 0;
            for (size_t k = 0; k < n; ++k) {
                sum += lhs(i, k) * mid(k, j) * rhs(j, k);
            }
            result(i, j) = sum;
        }
    }

    return result;
}


static Matrix<float> toMatrix(const Vertex& v) {
    Matrix<float> matrix(4, 1); // Create a 4x1 matrix (column vector)
    matrix(0, 0) = v.x;
    matrix(1, 0) = v.y;
    matrix(2, 0) = v.z;
    matrix(3, 0) = 1.0f; // Homogeneous coordinate
    return matrix;
}

static Vertex toVertex(const Matrix<float>& m) {
    return {m(0, 0) / m(3, 0), m(1, 0) / m(3, 0), m(2, 0) / m(3, 0)};
}

#endif //ZAPOCTAK2_0_DATATYPES_H
