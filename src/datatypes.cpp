//
// Created by Martin Lejko on 31/01/2024.
//

#include "datatypes.h"
#include <cmath>
#include "logging.h"

int Vec2::crossProduct(const Vec2 &v) const {
    return x * v.y - y * v.x;
}

Vec3 Vec3::crossProduct(const Vec3& v) const {
    return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

Vec3 stringToVec3(const std::string& str) {
    std::istringstream iss(str);
    float x, y, z;
    char delimiter;
    if (!(iss >> x >> delimiter >> y >> delimiter >> z)) {
        logger->error("Invalid format for Vec3 string");
        exit(1);
    }
    return Vec3(x, y, z);
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




