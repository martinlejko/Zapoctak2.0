//
// Created by Martin Lejko on 31/01/2024.
//

#include "parser.h"
#include <fstream>
#include <sstream>

bool Parser::isValidFile(std::string &filename) {
    std::ifstream  file(filename);
    return file.is_open();
}

void Parser::parseFaceElement(const std::string& line , std::unordered_map<int, std::vector<FaceElement>>& faces, int& index) {
    std::istringstream iss(line);
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
    faces.emplace(index++, face);
}

void Parser::parseVertex(const std::string& line, std::unordered_map<int, Vertex>& vertices, int& index) {
    Vertex vertex{};
    std::istringstream iss(line);
    iss >> vertex.x >> vertex.y >> vertex.z;
    vertices.emplace(index++, vertex);
}

void Parser::parseVertexNormal(const std::string& line, std::unordered_map<int, VertexNormal>& vertexNormals, int& index) {
    VertexNormal vertexNormal{};
    std::istringstream iss(line);
    iss >> vertexNormal.x >> vertexNormal.y >> vertexNormal.z;
    vertexNormals.emplace(index++, vertexNormal);
}

void Parser::parseUVVector(const std::string& line, std::unordered_map<int, UVVector>& uvVectors, int& index) {
    UVVector uvVector{};
    std::istringstream iss(line);
    iss >> uvVector.u >> uvVector.v;
    uvVectors.emplace(index++, uvVector);
}

WaveFrontData Parser::parseObjFile(std::string &filename) {
    if (!isValidFile(filename)) {
        throw std::runtime_error("Error: Unable to open or read the file " + filename); //do i have to catch it?
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
            parseVertex(line, objData.vertices, vertexIndex);
        } else if (type == "vn") {
            parseVertexNormal(line, objData.vertexNormals, vertexNormalIndex);
        } else if (type == "vt") {
            parseUVVector(line, objData.uvVectors, uvIndex);
        } else if (type == "f") {
            parseFaceElement(line, objData.faces, faceIndex);
        }
    }
    return objData;
}