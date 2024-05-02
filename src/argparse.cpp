//
// Created by Martin Lejko on 02/05/2024.
//

#include "argparse.h"
#include "logging.h"
#include "datatypes.h"

bool isValidDrawType(const std::string& drawType) {
    return drawType == "lines" || drawType == "colortriangle" || drawType == "texture" || drawType == "light";
}

Args parseArgs(int argc, char* argv[]) {
    Args args;

    args.zBufferFlag = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-model" && i + 1 < argc) {
            args.modelFile = argv[++i];
        } else if (arg == "-texture" && i + 1 < argc) {
            args.textureFile = argv[++i];
        } else if (arg == "-zbuffer") {
            args.zBufferFlag = true;
        } else if (arg == "-lightdirection" && i + 1 < argc) {
            try {
                Vec3 lightDir = stringToVec3(argv[++i]);
                args.lightDirection = lightDir;
            } catch (const std::invalid_argument& e) {
                logger->error("Error: Invalid light direction format. Expected format: -lightdirection [x,y,z]");
                exit(1);
            }
        } else if (arg == "-drawtype" && i + 1 < argc) {
            args.drawType = argv[++i];
        }
    }

    if (args.modelFile.empty()) {
        logger->error("Error: Model file is missing. Expected usage: -model [model_file]");
        exit(1);
    }
    if (!isValidDrawType(args.drawType) || args.drawType.empty()) {
        logger->error("Error: Draw type is missing. Expected usage: -drawtype [lines|colortriangle|texture|light]");
        exit(1);
    }


    logger->debug("Model file: {}", args.modelFile);
    logger->debug("Texture file: {}", args.textureFile);
    logger->debug("Draw type: {}", args.drawType);
    std::ostringstream lightDirectionStr;
    lightDirectionStr << "lightdir: {" << args.lightDirection[0] << ", "
                      << args.lightDirection[1] << ", "
                      << args.lightDirection[2] << "}";
    logger->debug(lightDirectionStr.str());
    logger->debug("ZBuffer flag: {}", args.zBufferFlag);

    return args;
}