//
// Created by Martin Lejko on 31/01/2024.
//

#include "parser.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool Parser::isValidFile(std::string &filename) {
    std::ifstream  file(filename);
    return file.is_open();
}

void Parser::printDataInfo(WaveFrontData &data) {
    std::cout << "Vertices: " << data.vertices.size() << std::endl;
    std::cout << "Vertex normals: " << data.vertexNormals.size() << std::endl;
    std::cout << "UV vectors: " << data.uvVectors.size() << std::endl;
    std::cout << "Faces: " << data.faces.size() << std::endl;
}

void Parser::printNthFace(WaveFrontData &data, int n) {
    auto it = data.faces.find(n);
    std::cout << "Face " << n << ": " << std::endl;
    for (const auto& faceElement : it->second) {
        std::cout << "  Vertex Index: " << faceElement.vertexIndex << " | Texture Index: " <<
                  faceElement.textureIndex << " | Normal Index: " << faceElement.normalIndex << std::endl;
    }
}

void Parser::printNthVertex(WaveFrontData &data, int n) {
    auto it = data.vertices.find(n);
    std::cout << "Vertex " << n << ": " << std::endl;
    std::cout << it->second.x << " " << it->second.y << " " << it->second.z << std::endl;
}

WaveFrontData Parser::parseObjFile(std::string &filename) {
    if (!isValidFile(filename)) {
        throw std::runtime_error("Error: Unable to open or read the file " + filename); //do I have to catch it?
    }

    WaveFrontData objData;
    std::ifstream file(filename);
    std::string line;
    int vertexIndex = 1;
    int vertexNormalIndex = 1;
    int faceIndex = 1;
    int uvIndex = 1;
    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "v") {
            Vertex vertex{};
            iss >> vertex.x >> vertex.y >> vertex.z;
            objData.vertices.emplace(vertexIndex++, vertex);
        } else if (type == "vn") {
            VertexNormal vertexNormal{};
            iss >> vertexNormal.x >> vertexNormal.y >> vertexNormal.z;
            objData.vertexNormals.emplace(vertexNormalIndex++, vertexNormal);
        } else if (type == "vt") {
            UVVector uvVector{};
            iss >> uvVector.x >> uvVector.y;
            objData.uvVectors.emplace(uvIndex++, uvVector);
        } else if (type == "f") {
            std::vector<FaceElement> face;
            FaceElement faceElement{};
            while (iss >> faceElement.vertexIndex) {
                char separator;
                if (iss.peek() == '/') {
                    iss >> separator >> faceElement.textureIndex;
                    if (iss.peek() == '/') {
                        iss >> separator >> faceElement.normalIndex;
                    } else {
                        faceElement.normalIndex = 0; // Default value if not provided
                    }
                } else {
                    faceElement.textureIndex = 0; // Default value if not provided
                    faceElement.normalIndex = 0; // Default value if not provided
                }
                face.push_back(faceElement);
            }
            if (face.size() == 9){
                throw std::runtime_error("Error: Face element is not a triangle");
            }
            objData.faces.emplace(faceIndex++, face);
        }

    }
    return objData;
}