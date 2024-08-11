//
// Created by Martin Lejko on 11/08/2024.
//

#ifndef IPARSERSTRATEGY_H
#define IPARSERSTRATEGY_H

#include "datatypes.h"
#include <string>

class IParserStrategy {
public:
    virtual ~IParserStrategy() = default;
    virtual WaveFrontData parse(const std::string& filename) = 0;
};

#endif // IPARSERSTRATEGY_H
