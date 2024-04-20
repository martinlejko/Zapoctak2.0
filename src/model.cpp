//
// Created by Martin Lejko on 31/01/2024.
//

#include "model.h"
#include "render.h"
#include "parser.h"
#include "datatypes.h"
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
        TriangleData triangle = {
                objData.vertices[face.second[0].vertexIndex],
                objData.vertices[face.second[1].vertexIndex],
                objData.vertices[face.second[2].vertexIndex]
        };
        TGAColor colorful = TGAColor(rand() % 255, rand() % 255, rand() % 255, 255);
        drawTriangle(triangle, colorful, image);
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

        TriangleData triangle = {
                objData.vertices[vIdx1],
                objData.vertices[vIdx2],
                objData.vertices[vIdx3]
        };

        Vec3 u = Vec3(originalVertices[vIdx3], originalVertices[vIdx1]);
        Vec3 v = Vec3(originalVertices[vIdx2], originalVertices[vIdx1]);
        Vec3 normal = u.crossProduct(v);

        normal.normalize();
        float intensity = normal.dotProduct(lightDirection);

        if (intensity > 0) {
            TGAColor color = TGAColor(255 * intensity, 255 * intensity, 255 * intensity, 255);
            if (useZBuffer) {
                drawTriangleZ(triangle, zBuffer, color, image);
            } else {
                drawTriangle(triangle, color, image);
            }
        }
    }
}

void Model::drawModelWithTexture(TGAImage &image, Vec3 lightDirection, bool useZBuffer) {
//    lightDirection.normalize();
    Vec3 eye(3,1,1);
    Vec3 center(0,0,0);

    if(useZBuffer){
        for (int i = width * height; i--; zBuffer[i] = std::numeric_limits<float>::lowest());
    }
//    Matrix<float> ModelView = Matrix<float>::lookat(eye, center, Vec3(0, 1, 0));
    Matrix<float> Projection = Matrix<float>::identity(4);
    Matrix<float> ViewPort = Matrix<float>::viewport((width - width*2)/2, (height - height*2)/2, width*2, height*2); // this one is good cetered
//    Matrix<float> ViewPort = Matrix<float>::viewport(width/8, height/8, width*3/4, height*3/4);
//    Projection(3, 2) = -1.0f / 3.0f;
    Projection(3,2) = -1.0f / (eye - center).norm();
//    float viewAngle = 0.5f; // Example viewing angle
//    float rotationAngle = 0.5f; // Example rotation angle around y-axis
//
//    Matrix<float> ViewAngleRotation(4, 4);
//    ViewAngleRotation(0, 0) = cos(viewAngle);
//    ViewAngleRotation(0, 1) = 0;
//    ViewAngleRotation(0, 2) = -sin(viewAngle);
//    ViewAngleRotation(1, 0) = 0;
//    ViewAngleRotation(1, 1) = 1.0f;
//    ViewAngleRotation(1, 2) = 0;
//    ViewAngleRotation(2, 0) = sin(viewAngle);
//    ViewAngleRotation(2, 1) = 0;
//    ViewAngleRotation(2, 2) = cos(viewAngle);
//    ViewAngleRotation(3, 3) = 1.0f;
//
//    Matrix<float> YAxisRotation(4, 4);
//    YAxisRotation(0, 0) = cos(rotationAngle);
//    YAxisRotation(0, 2) = sin(rotationAngle);
//    YAxisRotation(1, 1) = 1.0f;
//    YAxisRotation(2, 0) = -sin(rotationAngle);
//    YAxisRotation(2, 2) = cos(rotationAngle);
//    YAxisRotation(3, 3) = 1.0f;
//
//    Matrix<float> totalRotation = YAxisRotation * ViewAngleRotation;
//
//    ViewPort = totalRotation * ViewPort;
    Matrix<float> View = (ViewPort * Projection);
    for(auto &face : objData.faces) {
        int vIdx1 = face.second[0].vertexIndex;
        int vIdx2 = face.second[1].vertexIndex;
        int vIdx3 = face.second[2].vertexIndex;

        int uvIdx1 = face.second[0].textureIndex;
        int uvIdx2 = face.second[1].textureIndex;
        int uvIdx3 = face.second[2].textureIndex;

        TriangleData triangle = {
                objData.vertices[vIdx1],
                objData.vertices[vIdx2],
                objData.vertices[vIdx3]
        };

        triangle.v1 = toVertex(View * toMatrix(triangle.v1));
        triangle.v2 = toVertex(View * toMatrix(triangle.v2));
        triangle.v3 = toVertex(View * toMatrix(triangle.v3));

        triangle.v1.x = (triangle.v1.x + 1) / 1000;
        triangle.v1.y = (triangle.v1.y + 1) / 1000;
        triangle.v2.x = (triangle.v2.x + 1) / 1000;
        triangle.v2.y = (triangle.v2.y + 1) / 1000;
        triangle.v3.x = (triangle.v3.x + 1) / 1000;
        triangle.v3.y = (triangle.v3.y + 1) / 1000;

        UVTriangleData uvTriangle = {
                objData.uvVectors[uvIdx1],
                objData.uvVectors[uvIdx2],
                objData.uvVectors[uvIdx3]
        };

        Vec3 u = Vec3(originalVertices[vIdx3], originalVertices[vIdx1]);
        Vec3 v = Vec3(originalVertices[vIdx2], originalVertices[vIdx1]);
        Vec3 normal = u.crossProduct(v);

        normal.normalize();
        float intensity = normal.dotProduct(lightDirection);

        if (intensity > 0) {
            if(useZBuffer){

                drawTriangleTextureZ(triangle, uvTriangle, intensity, zBuffer, texture, image);
            }
        }
    }
}
