//
// Created by Martin Lejko on 31/01/2024.
//

#ifndef ZAPOCTAK2_0_DATATYPES_H
#define ZAPOCTAK2_0_DATATYPES_H

#include <unordered_map>

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
    float u;
    float v;
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

#endif //ZAPOCTAK2_0_DATATYPES_H
