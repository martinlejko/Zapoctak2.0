//
// Created by Martin Lejko on 31/01/2024.
//

#include "model.h"
#include "parser.h"
#include "render.h"
#include <iostream>

Model::Model(std::string filename) {
    //working with the obj file, tga texture
    Parser parser;
    objData = parser.parseObjFile(filename);
    std::cout << "Model created" << std::endl;

    //testing the parser
    parser.printDataInfo(objData);
    parser.printNthVertex(objData, 1);
    parser.printNthFace(objData, 1);

}