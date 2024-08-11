//
// Created by Martin Lejko on 31/01/2024.
//

#include "parser.h"
#include "logging.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <algorithm>

bool Parser::isValidFile(std::string &filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        logger->info("File {} is valid and open.", filename);
        return true;
    } else {
        logger->error("Failed to open file {}.", filename);
        return false;
    }
}
void Parser::printDataInfo(WaveFrontData &data) {
    logger->debug("Printing data info:");
    logger->debug("Vertices: {}", data.vertices.size());
    logger->debug("Vertex normals: {}", data.vertexNormals.size());
    logger->debug("UV vectors: {}", data.uvVectors.size());
    logger->debug("Faces: {}", data.faces.size());
}
void Parser::printNthFace(WaveFrontData &data, int n) {
    auto it = data.faces.find(n);
    logger->debug("Face {}: ", n);
    for (const auto& faceElement : it->second) {
        logger->debug("  Vertex Index: {} | Texture Index: {} | Normal Index: {}",
                     faceElement.vertexIndex, faceElement.textureIndex, faceElement.normalIndex);
    }
}


void Parser::printNthVertex(WaveFrontData &data, int n) {
    auto it = data.vertices.find(n);
    logger->info("Vertex {}: ", n);
    logger->info("{} {} {}", it->second.x, it->second.y, it->second.z);
}

void Parser::normalizeVectors(WaveFrontData &data) {
    // Getting min and max value of vectors
    std::vector<float> min_values(3, std::numeric_limits<float>::max());
    std::vector<float> max_values(3, std::numeric_limits<float>::lowest());

    for (const auto& vertexPair : data.vertices) {
        const Vertex& v = vertexPair.second;  // Access the Vertex structure from the unordered_map
        for (int i = 0; i < 3; ++i) {
            min_values[i] = std::min(min_values[i], v.x);
            max_values[i] = std::max(max_values[i], v.x);
        }
    }

    // Normalize the vertex coordinates to be between -1 and 1
    for (auto& vertexPair : data.vertices) {
        Vertex& v = vertexPair.second;
        for (int i = 0; i < 3; ++i) {
            v.x = 2.0f * (v.x - min_values[i]) / (max_values[i] - min_values[i]) - 1.0f;
        }
        for (int i = 0; i < 3; ++i) {
            v.y = 2.0f * (v.y - min_values[i]) / (max_values[i] - min_values[i]) - 1.0f;
        }
        for (int i = 0; i < 3; ++i) {
            v.z = 2.0f * (v.z - min_values[i]) / (max_values[i] - min_values[i]) - 1.0f;
        }
    }
    logger->info("Vectors normalized.");
}