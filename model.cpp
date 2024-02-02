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
    parser.printNthVertex(objData, 1);
    projectVerts(width, height, objData.vertices);
    parser.printNthVertex(objData, 1);
    //inicilization of the zBuffer
    inicializeZBuffer();
    //testing the parser
//    std::cout << "Model created" << std::endl;
//    parser.printDataInfo(objData);
//    parser.printNthVertex(objData, 1);
//    parser.printNthFace(objData, 1);
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
