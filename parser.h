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
    static WaveFrontData parseObjFile(std::string &filename);
    static void printDataInfo(WaveFrontData &data);
    static void printNthVertex(WaveFrontData &data, int n);
    static void printNthFace(WaveFrontData &data, int n);
private:
    //functions for parsing Obj file
    static bool isValidFile(std::string &filename);
    void parseTextureFile(std::string &filename);
};


#endif //ZAPOCTAK2_0_PARSER_H
