//
// Created by Martin Lejko on 31/01/2024.
//

#include "model.h"
#include "render.h"
#include "parser.h"
#include "render.h"
#include "tgaimage.h"
#include <iostream>

Model::Model(int width, int height, std::string filename) : width(width), height(height){
    //working with the obj file, tga texture
    Parser parser;
    objData = parser.parseObjFile(filename);
    originalVertices = objData.vertices;
    projectVerts(width, height, objData.vertices);
    //inicilization of the zBuffer
    inicializeZBuffer();

    //testing the parser
//    std::cout << "Model created" << std::endl;
//    parser.printDataInfo(objData);
//    parser.printNthFace(objData, 1);
//    parser.printNthVertex(objData, 1);
}

void Model::inicializeZBuffer() {
    zBuffer = new int[width * height];
    for (int i = 0; i < width * height; i++) {
        zBuffer[i] = std::numeric_limits<int>::min();
    }
}

void Model::drawModelLinesOnly(TGAImage &image, TGAColor &color) {
    //draws the model with lines only
    for (auto &face : objData.faces) {
        for (int i = 0; i < 3; i++) {
            int v1 = face.second[i].vertexIndex;
            int v2 = face.second[(i + 1) % 3].vertexIndex;
            drawLine(objData.vertices[v1], objData.vertices[v2], color, image);
        }
    }
}

void Model::drawModelColorfulTriangles(TGAImage &image) {
    //draws the model with simple triangles
    for (auto &face : objData.faces) {
        TGAColor colorful = TGAColor(rand() % 255, rand() % 255, rand() % 255, 255);
        drawTriangle(objData.vertices[face.second[0].vertexIndex], objData.vertices[face.second[1].vertexIndex],
                     objData.vertices[face.second[2].vertexIndex], colorful, image);
    }
}

void Model::drawModelWithShadows(TGAImage &image, Vec3 lightDirection) {
    //draws the model with shadows
    for (auto &face : objData.faces) {
        int vIdx1 = face.second[0].vertexIndex;
        int vIdx2 = face.second[1].vertexIndex;
        int vIdx3 = face.second[2].vertexIndex;

        Vec3 u = Vec3(originalVertices[vIdx3], originalVertices[vIdx1]);
        Vec3 v = Vec3(originalVertices[vIdx2], originalVertices[vIdx1]);
        Vec3 normal = u.crossProduct(v);

        normal.normalize();
        float intensity = normal.dotProduct(lightDirection);
        
        if (intensity > 0) {
            TGAColor color = TGAColor(255 * intensity, 255 * intensity, 255 * intensity, 255);
            drawTriangle(objData.vertices[vIdx1], objData.vertices[vIdx2],
                         objData.vertices[vIdx3], color, image);
        }
    }
}