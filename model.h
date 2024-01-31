//
// Created by Martin Lejko on 31/01/2024.
//

#ifndef ZAPOCTAK2_0_MODEL_H
#define ZAPOCTAK2_0_MODEL_H

#include "datatypes.h"
#include <string>
#include <unordered_map>


class Model {
public:
    Model(std::string filename);
private:
    WaveFrontData objData;
};


#endif //ZAPOCTAK2_0_MODEL_H
