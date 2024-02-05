#include "tgaimage.h"
#include "model.h"
#include "render.h"
#include <limits>


const int width = 800;
const int height = 800;
TGAColor red = TGAColor(255, 0, 0, 255);
const Vec3 lightDirection = Vec3(0, 0, -1);

int main() {
    TGAImage image(width, height, TGAImage::RGB);
    Model model(width, height, "C:\\Users\\mlejko\\sandbox\\african_head.obj");
//    model.drawModelLinesOnly(image, red);
//    model.drawModelColorfulTriangles(image);
    model.drawModelWithShadows(image, lightDirection, true);
    image.flip_vertically();
    image.write_tga_file("output.tga");

    TGAImage zbimage(width, height, TGAImage::GRAYSCALE);
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++) {
            if (model.zBuffer[i + j * width] != std::numeric_limits<float>::lowest()) {
                zbimage.set(i, j, TGAColor(255,255,255, 1));
            }
        }
    }
    zbimage.flip_vertically();
    zbimage.write_tga_file("zbuffer.tga");
    return 0;
}


//tests the functionality

//const TGAColor white = TGAColor(255, 255, 255, 255);
//Vertex v1 = {80, 10, 10};
//Vertex v2 = {20, 70, 20};
//Vertex v3 = {70, 80, 30};
//drawSimpleTriangle(v1, v2, v3, image, zBuffer);
//drawLine(v1, v2, white, image);