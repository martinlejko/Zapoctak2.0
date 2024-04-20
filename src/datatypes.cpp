//
// Created by Martin Lejko on 31/01/2024.
//

#include "datatypes.h"
#include <cmath>

int Vec2::crossProduct(const Vec2 &v) const {
    return x * v.y - y * v.x;
}

Vec3 Vec3::crossProduct(const Vec3& v) const {
    return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

void Vec3::normalize() {
    float length = std::sqrt(x * x + y * y + z * z);
    if (length != 0) {
        x /= length;
        y /= length;
        z /= length;
    }
}

float Vec3::dotProduct(const Vec3& v) const {
    return x * v.x + y * v.y + z * v.z;
}

BoundingBox calculateBoundingBox(const WaveFrontData& objData) {
    BoundingBox boundingBox;
    boundingBox.minX = boundingBox.minY = boundingBox.minZ = std::numeric_limits<float>::max();
    boundingBox.maxX = boundingBox.maxY = boundingBox.maxZ = std::numeric_limits<float>::min();

    for (const auto& vertex : objData.vertices) {
        boundingBox.minX = std::min(boundingBox.minX, vertex.second.x);
        boundingBox.minY = std::min(boundingBox.minY, vertex.second.y);
        boundingBox.minZ = std::min(boundingBox.minZ, vertex.second.z);
        boundingBox.maxX = std::max(boundingBox.maxX, vertex.second.x);
        boundingBox.maxY = std::max(boundingBox.maxY, vertex.second.y);
        boundingBox.maxZ = std::max(boundingBox.maxZ, vertex.second.z);
    }

    return boundingBox;
}


