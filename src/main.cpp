#include "tgaimage.h"
#include "model.h"
#include <limits>
#include "datatypes.h"

//set the parameters that you want
const int width = 800;
const int height = 800;
Vec3 lightDirection = Vec3(0, 0, -1);
TGAColor blue = {255, 0, 0, 255};
int main() {
    //creation of the image that we are rendering onto
    Model model("/Users/martinlejko/Desktop/github/ZS2023/simple-software-renderer/objects/head/african_head.obj"); //provide the obj file to render

    //choose the render option
//    model.drawModelLinesOnly(blue);
//    model.drawModelColorfulTriangles();
//    model.drawModelWithShadows(lightDirection, false);
//    model.drawModelWithTexture(lightDirection, {0,0,3});

    for (float i = 0; i < 30; i++) {
//        for(float j = 0; j < 11; j++) {
//           for(float k = 0; k < 11; k++) {
               model.drawModelWithTexture(lightDirection, {1,1,i});
//           }
//        }
    }
    //zBuffer testing generation
    TGAImage zbimage(width, height, TGAImage::GRAYSCALE);
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++) {
            if (model.zBuffer[i + j * width] != std::numeric_limits<float>::lowest()) {
                zbimage.set(i, j, TGAColor{255,255,255, 1});
            }
        }
    }
    zbimage.write_tga_file("zbuffer.tga");
    return 0;
}
//also you can use drawline or drawTriangle functions