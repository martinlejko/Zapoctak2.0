//
// Created by Martin Lejko on 31/01/2024.
//

#include "datatypes.h"

int Vec2::crossProduct(const Vec2 &v) const {
    return x * v.y - y * v.x;
}

Vec3 Vec3::crossProduct(const Vec3& v) const {
    return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}
