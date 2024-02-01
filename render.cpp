//
// Created by Martin Lejko on 01/02/2024.
//

#include "render.h"
#include <iostream>

void drawLine(const Vertex &v1, const Vertex &v2, const TGAColor &color, TGAImage &image) {
//Bresenham's line algorithm
    int x = v1.x;
    int y = v1.y;
    int dx = abs(v2.x - v1.x);
    int dy = abs(v2.y - v1.y);
    int sx = v1.x < v2.x ? 1 : -1;
    int sy = v1.y < v2.y ? 1 : -1;
    int err = dx - dy;

    while (true) {
        //colors the pixel at (x, y) with the given color
        std::cout << "x: " << x << " y: " << y << std::endl;
        image.set(x, y, color);

        if (x == v2.x && y == v2.y) {
            break;
        }
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}


