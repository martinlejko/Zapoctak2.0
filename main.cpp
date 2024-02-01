#include "tgaimage.h"
#include "model.h"
#include "render.h"
#include "datatypes.h"

const TGAColor white = TGAColor(255, 255, 255, 255);

int main() {
    TGAImage image(800, 800, TGAImage::RGB);
    Model model("/Users/martinlejko/Desktop/github/Software-Renderer-From-Scratch/src/objects/african_head.obj");
    Vertex v1 = {80, 10, 10};
    Vertex v2 = {20, 70, 20};
    drawLine(v1, v2, white, image);
    image.flip_vertically();
    image.write_tga_file("output.tga");
    return 0;
}
