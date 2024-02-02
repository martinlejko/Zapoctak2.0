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
    int *zBuffer{};
private:
    void inicializeZBuffer();
    int width;
    int height;
    WaveFrontData objData;
};


#endif //ZAPOCTAK2_0_MODEL_H
