//
// Created by Martin Lejko on 31/01/2024.
//

#ifndef ZAPOCTAK2_0_PARSER_H
#define ZAPOCTAK2_0_PARSER_H

#include "datatypes.h"
#include "IParserStrategy.h"
#include <string>
#include <unordered_map>


class Parser {
public:
    explicit Parser(std::unique_ptr<IParserStrategy> strategy)
            : strategy(std::move(strategy)) {}
    void setStrategy(std::unique_ptr<IParserStrategy> newStrategy) {
        strategy = std::move(newStrategy);
    }
    WaveFrontData parseFile(const std::string& filename) {
        return strategy->parse(filename);
    }

    static WaveFrontData parseObjFile(std::string &filename);
    static void printDataInfo(WaveFrontData &data);
    static void printNthVertex(WaveFrontData &data, int n);
    static void printNthFace(WaveFrontData &data, int n);
    static void normalizeVectors(WaveFrontData &data);
private:
    std::unique_ptr<IParserStrategy> strategy;
    static bool isValidFile(std::string &filename);
};


#endif //ZAPOCTAK2_0_PARSER_H
