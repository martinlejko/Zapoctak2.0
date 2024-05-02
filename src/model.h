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
    Model(std::string filename, std::string textureFile = "", int width = 800, int height = 800);
    void drawModelLinesOnly(TGAColor &color);
    void drawModelColorfulTriangles();
    void drawModelWithShadows(Vec3 lightDirection, bool useZBuffer = true);
    void drawModelWithTexture(Vec3 lightDirection, bool useZBuffer = true);
    TGAImage texture;
    std::vector<float> zBuffer;
    static void printZBuffer(const std::vector<float>& zBuffer, int width, int height);
private:
    static  void loadTexture(const std::string& textureFile, TGAImage &image);
    int width;
    int height;
    TGAImage image;
    std::unordered_map<int, Vertex> originalVertices;
    WaveFrontData objData;
};


#endif //ZAPOCTAK2_0_MODEL_H
