//
// Created by Martin Lejko on 01/02/2024.
//

#include "render.h"
#include "datatypes.h"
#include "logging.h"
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

void projectVerts(int width, int height, std::unordered_map<int, Vertex>& vertices) {
    //project the vertices to the screen
    for (auto &vertex : vertices) {
        vertex.second.x = (int)((vertex.second.x + 1) * width / 2);
        vertex.second.y = (int)((vertex.second.y + 1) * height / 2);
    }
    logger->info("Vertices projected to the screen.");
}

Vec3 barycentric(const Vertex &v1, const Vertex &v2, const Vertex &v3, const Vertex &p) {
    Vec3 u = Vec3(v3.x - v1.x, v2.x - v1.x, v1.x - p.x);
    Vec3 v = Vec3(v3.y - v1.y, v2.y - v1.y, v1.y - p.y);
    Vec3 w = u.crossProduct(v);
    if (std::abs(w.z) < 1) return {-1, 1, 1};
    return {1.0f - (w.x + w.y) / w.z, w.y / w.z, w.x / w.z};
}

void drawTriangle(const TriangleData &triangle, const TGAColor &color, TGAImage &image) {
    const Vertex &v1 = triangle.v1;
    const Vertex &v2 = triangle.v2;
    const Vertex &v3 = triangle.v3;
    // bounding box
    int minX = std::min(v1.x, std::min(v2.x, v3.x));
    int minY = std::min(v1.y, std::min(v2.y, v3.y));
    int maxX = std::max(v1.x, std::max(v2.x, v3.x));
    int maxY = std::max(v1.y, std::max(v2.y, v3.y));

    Vertex current{};
    for (current.x = minX; current.x <= maxX; current.x++) {
        for (current.y = minY; current.y <= maxY; current.y++) {
            Vec3 bcCoords = barycentric(v1, v2, v3, current);
            if (bcCoords.x < 0 || bcCoords.y < 0 || bcCoords.z < 0) continue;
            image.set(current.x, current.y, color);
        }
    }
}

void drawTriangleZ(const TriangleData &triangle, std::vector<float>& zbuffer, const TGAColor& color, TGAImage& image) {
    const Vertex &v1 = triangle.v1;
    const Vertex &v2 = triangle.v2;
    const Vertex &v3 = triangle.v3;
    // bounding box
    int minX = std::min(v1.x, std::min(v2.x, v3.x));
    int minY = std::min(v1.y, std::min(v2.y, v3.y));
    int maxX = std::max(v1.x, std::max(v2.x, v3.x));
    int maxY = std::max(v1.y, std::max(v2.y, v3.y));

    Vertex current{};
    for (current.x = minX; current.x <= maxX; current.x++) {
        for (current.y = minY; current.y <= maxY; current.y++) {
            Vec3 bcCoords = barycentric(v1, v2, v3, current);
            if (bcCoords.x < 0 || bcCoords.y < 0 || bcCoords.z < 0) continue;
            float z = v1.z * bcCoords.x + v2.z * bcCoords.y + v3.z * bcCoords.z;
            int idx = current.x + current.y * image.width();

            if (idx >= 0 && idx < zbuffer.size() && zbuffer[idx] <= z ) {
                zbuffer[idx] = z;
                image.set(current.x, current.y, color);
            }
        }
    }
}

void drawTriangleTextureZ(const TriangleData &triangle, const UVTriangleData &uvTriangle, float intensity, std::vector<float> &zbuffer, TGAImage &texture, TGAImage &image) {
    const Vertex &v1 = triangle.v1;
    const Vertex &v2 = triangle.v2;
    const Vertex &v3 = triangle.v3;
    // bounding box
    int minX = std::min(v1.x, std::min(v2.x, v3.x));
    int minY = std::min(v1.y, std::min(v2.y, v3.y));
    int maxX = std::max(v1.x, std::max(v2.x, v3.x));
    int maxY = std::max(v1.y, std::max(v2.y, v3.y));

    Vertex current{};
    for (current.x = minX; current.x <= maxX; current.x++) {
        for (current.y = minY; current.y <= maxY; current.y++) {
            Vec3 bcCoords = barycentric(v1, v2, v3, current);
            if (bcCoords.x < 0 || bcCoords.y < 0 || bcCoords.z < 0) continue;
            const UVVector &uv1 = uvTriangle.uv1;
            const UVVector &uv2 = uvTriangle.uv2;
            const UVVector &uv3 = uvTriangle.uv3;
            UVVector uvP = {
                    static_cast<float>(uv1.x * bcCoords.x + uv2.x * bcCoords.y + uv3.x * bcCoords.z),
                    static_cast<float>(uv1.y * bcCoords.x + uv2.y * bcCoords.y + uv3.y * bcCoords.z)
            };
            TGAColor color = texture.get(static_cast<int>(uvP.x*texture.width()), static_cast<int>(uvP.y * texture.height()));
            float z = v1.z * bcCoords.x + v2.z * bcCoords.y + v3.z * bcCoords.z;
            int idx = current.x + current.y * image.width();
            if (idx >= 0 && idx < zbuffer.size() && zbuffer[idx] <= z ) {
                zbuffer[idx] = z;
                TGAColor newColor = {static_cast<std::uint8_t>(color[0] * intensity),
                                     static_cast<std::uint8_t>(color[1] * intensity),
                                     static_cast<std::uint8_t>(color[2] * intensity),
                                     255};
                image.set(current.x, current.y, newColor);
            }
        }
    }
}