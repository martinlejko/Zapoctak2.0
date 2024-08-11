//
// Created by Martin Lejko on 11/08/2024.
//

#ifndef SIMPLE_SOFTWARE_RENDERER_WAVEFRONTPARSERSTRATEGY_H
#define SIMPLE_SOFTWARE_RENDERER_WAVEFRONTPARSERSTRATEGY_H

#include "IParserStrategy.h"
#include "datatypes.h"
#include "logging.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

class WaveFrontParserStrategy : public IParserStrategy {
public:
    WaveFrontData parse(const std::string& filename) override {
        logger->info("Parsing using WaveFrontParserStrategy.");
        if (!isValidFile(filename)) {
            logger->error("Error: Unable to open or read the file {}", filename);
            throw std::runtime_error("Error: Unable to open or read the file " + filename);
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
                if (face.size() == 9) {
                    logger->error("Error: Face element is not a triangle");
                    throw std::runtime_error("Error: Face element is not a triangle");
                }
                objData.faces.emplace(faceIndex++, face);
            }
        }

        logger->info("File {} parsed successfully.", filename);
        return objData;
    }

private:
    bool isValidFile(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            logger->info("File {} is valid and open.", filename);
            return true;
        } else {
            logger->error("Failed to open file {}.", filename);
            return false;
        }
    }
};

#endif //SIMPLE_SOFTWARE_RENDERER_WAVEFRONTPARSERSTRATEGY_H