//
// Created by Martin Lejko on 31/01/2024.
//

#include "model.h"
#include "render.h"
#include "parser.h"
#include "render.h"
#include "tgaimage.h"
#include <iostream>
#include <limits>

Model::Model(int width, int height, std::string filename) : width(width), height(height) {
    //working with the obj file, tga texture
    Parser parser;
    objData = parser.parseObjFile(filename);
//    parser.normalizeVectors(objData);
    originalVertices = objData.vertices;
    projectVerts(width, height, objData.vertices);

    //zBuffer space allocation
    zBuffer.resize(width * height);

    //loading texture
    loadTexture(filename, texture);


    //testing the parser
//    std::cout << "Model created" << std::endl;
//    parser.printDataInfo(objData);
//    parser.printNthFace(objData, 1);
//    parser.printNthVertex(objData, 1);
}


void Model::loadTexture(std::string filename, TGAImage &image) {
    std::string textureFile = filename.substr(0, filename.size() - 4) + "_diffuse.tga"; // -4 because of .obj
    std::cout << "texture file " << textureFile << " loading " << (image.read_tga_file(textureFile.c_str()) ? "ok" : "failed") << std::endl;
    image.flip_vertically();
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

void Model::printZBuffer(const std::vector<float> zbuffer, int width, int height) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = x + y * width;
            if (zbuffer[idx] != std::numeric_limits<float>::min()) {
                std::cout << "1 ";
            }
        }
    }
}

void Model::drawModelWithShadows(TGAImage &image, Vec3 lightDirection, bool useZBuffer) {
    if(useZBuffer) {
        for (int i = width * height; i--; zBuffer[i] = std::numeric_limits<float>::lowest());
    }
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
            if (useZBuffer) {
                drawTriangleZ(objData.vertices[vIdx1], objData.vertices[vIdx2], objData.vertices[vIdx3], zBuffer, color, image);
            } else {
                drawTriangle(objData.vertices[vIdx1], objData.vertices[vIdx2], objData.vertices[vIdx3], color, image);
            }
        }
    }
}

void Model::drawModelWithTexture(TGAImage &image, Vec3 lightDirection, bool useZBuffer) {
    if(useZBuffer){
        for (int i = width * height; i--; zBuffer[i] = std::numeric_limits<float>::lowest());
    }
    for(auto &face : objData.faces) {
        int vIdx1 = face.second[0].vertexIndex;
        int vIdx2 = face.second[1].vertexIndex;
        int vIdx3 = face.second[2].vertexIndex;

        int uvIdx1 = face.second[0].textureIndex;
        int uvIdx2 = face.second[1].textureIndex;
        int uvIdx3 = face.second[2].textureIndex;

        Vec3 u = Vec3(originalVertices[vIdx3], originalVertices[vIdx1]);
        Vec3 v = Vec3(originalVertices[vIdx2], originalVertices[vIdx1]);
        Vec3 normal = u.crossProduct(v);

        normal.normalize();
        float intensity = normal.dotProduct(lightDirection);

        if (intensity > 0) {
            if(useZBuffer){
                drawTriangleTextureZ(objData.vertices[vIdx1], objData.vertices[vIdx2], objData.vertices[vIdx3], objData.uvVectors[uvIdx1], objData.uvVectors[uvIdx2], objData.uvVectors[uvIdx3], intensity, zBuffer, texture, image);
            }
        }
    }
}