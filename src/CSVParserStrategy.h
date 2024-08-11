//
// Created by Martin Lejko on 11/08/2024.
//

#ifndef SIMPLE_SOFTWARE_RENDERER_CSVPARSERSTRATEGY_H
#define SIMPLE_SOFTWARE_RENDERER_CSVPARSERSTRATEGY_H

#include "IParserStrategy.h"
#include "logging.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

class CSVParserStrategy : public IParserStrategy {
public:
    WaveFrontData parse(const std::string& filename) override {
        logger->info("Parsing using CSVParserStrategy.");
        WaveFrontData csvData;
        std::ifstream file(filename);
        if (!file.is_open()) {
            logger->error("Error: Unable to open or read the file {}", filename);
            throw std::runtime_error("Error: Unable to open or read the file " + filename);
        }

        std::string line;
        int lineNumber = 0;

        while (getline(file, line)) {
            std::istringstream iss(line);
            std::string value;
            std::vector<float> parsedValues;

            while (getline(iss, value, ',')) {
                try {
                    parsedValues.push_back(std::stof(value));
                } catch (const std::invalid_argument& e) {
                    logger->error("Error: Invalid number in line {}: {}", lineNumber, value);
                    throw std::runtime_error("Error: Invalid number in CSV file");
                }
            }

            if (parsedValues.size() == 3) {
                Vertex vertex{};
                vertex.x = parsedValues[0];
                vertex.y = parsedValues[1];
                vertex.z = parsedValues[2];
                csvData.vertices.emplace(++lineNumber, vertex);
            } else {
                logger->warn("Warning: Line {} does not have exactly 3 values: {}", lineNumber, line);
            }
        }

        logger->info("CSV file {} parsed successfully.", filename);
        return csvData;
    }
};

#endif //SIMPLE_SOFTWARE_RENDERER_CSVPARSERSTRATEGY_H
