//
// Created by Martin Lejko on 31/01/2024.
//

#ifndef ZAPOCTAK2_0_PARSER_H
#define ZAPOCTAK2_0_PARSER_H

#include "datatypes.h"
#include <string>
#include <unordered_map>


class Parser {
public:
    WaveFrontData parseObjFile(std::string &filename);
private:
    void parseTextureFile(std::string &filename);
    static bool isValidFile(std::string &filename);
    void parseFaceElement(const std::string& line, std::unordered_map<int, std::vector<FaceElement>>& faces, int& index);
    void parseVertex(const std::string& line, std::unordered_map<int, Vertex>& vertices, int& index);
    void parseVertexNormal(const std::string& line, std::unordered_map<int, VertexNormal>& vertexNormals, int& index);
    void parseUVVector(const std::string& line, std::unordered_map<int, UVVector>& uvVectors, int& index);
};


#endif //ZAPOCTAK2_0_PARSER_H
