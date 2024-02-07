//
// Created by Martin Lejko on 31/01/2024.
//

#ifndef ZAPOCTAK2_0_DATATYPES_H
#define ZAPOCTAK2_0_DATATYPES_H

#include <unordered_map>
#include <vector>

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
};

#endif //ZAPOCTAK2_0_DATATYPES_H
