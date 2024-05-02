//
// Created by Martin Lejko on 02/05/2024.
//

#ifndef SIMPLE_SOFTWARE_RENDERER_ARGPARSE_H
#define SIMPLE_SOFTWARE_RENDERER_ARGPARSE_H
#include "datatypes.h"

Args parseArgs(int argc, char* argv[]);
bool isValidDrawType(const std::string& drawType);


#endif //SIMPLE_SOFTWARE_RENDERER_ARGPARSE_H
