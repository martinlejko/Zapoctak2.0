//
// Created by Martin Lejko on 31/01/2024.
//

#ifndef ZAPOCTAK2_0_MODEL_H
#define ZAPOCTAK2_0_MODEL_H

#include "datatypes.h"
#include "tgaimage.h"
#include <string>
#include <unordered_map>


class Model {
public:
    Model(int width, int height, std::string filename);
    void drawModelLinesOnly(TGAImage &image, TGAColor &color);
    void drawModelColorfulTriangles(TGAImage &image);
    void drawModelWithShadows(TGAImage &image, Vec3 lightDirection, bool useZBuffer);
    void drawModelWithTexture(TGAImage &image, Vec3 lightDirection, bool useZBuffer);
    TGAImage texture;
    std::vector<float> zBuffer;
    TGAColor diffuse(UVVector uvP);
    void printZBuffer(const std::vector<float> zBuffer, int width, int height);
private:
    void loadTexture(std::string filename, TGAImage &image);
    int width;
    int height;
    std::unordered_map<int, Vertex> originalVertices;
    WaveFrontData objData;
};


#endif //ZAPOCTAK2_0_MODEL_H
