#pragma once

#include <cmath>

namespace EcoSystem {

// Вектор для представления позиций и направлений
struct Vector3 {
    float x, y, z;
    
    Vector3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) 
        : x(_x), y(_y), z(_z) {}
    
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
    
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
    
    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    
    Vector3& operator-=(const Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    
    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    
    float lengthSquared() const {
        return x * x + y * y + z * z;
    }
    
    Vector3 normalized() const {
        float len = length();
        if (len > 0.0f) {
            return Vector3(x / len, y / len, z / len);
        }
        return Vector3();
    }
    
    float distanceTo(const Vector3& other) const {
        return (*this - other).length();
    }
    
    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    Vector3 cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    
    bool operator==(const Vector3& other) const {
        const float epsilon = 0.0001f;
        return std::abs(x - other.x) < epsilon &&
               std::abs(y - other.y) < epsilon &&
               std::abs(z - other.z) < epsilon;
    }
    
    bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }
};

} // namespace EcoSystem
