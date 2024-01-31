#include "tgaimage.h"
#include "model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

int main() {
    TGAImage image(100, 100, TGAImage::RGB);
    image.set(52, 41, red);
    image.flip_vertically();
    image.write_tga_file("output.tga");
    Model model("/Users/martinlejko/Desktop/github/Software-Renderer-From-Scratch/src/objects/african_head.obj");
    return 0;
}
