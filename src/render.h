//
// Created by Martin Lejko on 01/02/2024.
//

#ifndef ZAPOCTAK2_0_RENDER_H
#define ZAPOCTAK2_0_RENDER_H

#include "datatypes.h"
#include "tgaimage.h"
#include <memory>


void drawLine(const Vertex& v1, const Vertex& v2, const TGAColor& color, TGAImage& image);

void projectVerts(int width, int height, std::unordered_map<int, Vertex>& vertices);
Vec3 barycentric(const TriangleData &triangle, const Vertex &p);
void drawTriangle(const TriangleData &triangle, const TGAColor &color, TGAImage &image);
void drawTriangleZ(const TriangleData &triangle, std::vector<float> &zbuffer, const TGAColor &color, TGAImage &image);
void drawTriangleTextureZ(const TriangleData &triangle, const UVTriangleData &uvTriangle, float intensity, std::vector<float> &zbuffer, TGAImage &texture, TGAImage &image);
#endif //ZAPOCTAK2_0_RENDER_H
