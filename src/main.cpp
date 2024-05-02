#include "tgaimage.h"
#include "model.h"
#include <limits>
#include "datatypes.h"
#include "logging.h"
#include "argparse.h"

//set the parameters that you want
const int width = 800;
const int height = 800;
Vec3 lightDirection = Vec3(0, 0, -1);
TGAColor blue = {255, 0, 0, 255};
int main(int argc, char* argv[]) {
    setupLogger();
    Args args = parseArgs(argc, argv);

    Model model(args.modelFile, args.textureFile);
    if (args.drawType == "lines") {
        model.drawModelLinesOnly(blue);
    } else if (args.drawType == "colortriangle") {
        model.drawModelColorfulTriangles();
    } else if (args.drawType == "texture") {
        model.drawModelWithTexture(args.lightDirection);
    } else if (args.drawType == "light") {
        model.drawModelWithShadows(args.lightDirection, args.zBufferFlag);
    }

    //zBuffer testing generation
    TGAImage zbimage(width, height, TGAImage::GRAYSCALE);
    logger->info("ZBuffer testing");
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++) {
            if (model.zBuffer[i + j * width] != std::numeric_limits<float>::lowest()) {
                zbimage.set(i, j, TGAColor{255,255,255, 1});
            }
        }
    }
    zbimage.write_tga_file("zbuffer.tga");
    logger->info("ZBuffer testing completed and file saved as zbuffer.tga");

    return 0;
}
//also you can use drawline or drawTriangle functions