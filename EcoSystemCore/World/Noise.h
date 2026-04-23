#pragma once

#include <cmath>
#include <algorithm>

namespace EcoSystem {

// Простая реализация шума Перлина для генерации ландшафта
class PerlinNoise {
private:
    int m_Permutation[512];
    
    // Псевдослучайная перестановка
    void shuffle(int* arr, int n, unsigned int seed) {
        // Простой генератор на основе seed
        unsigned int state = seed;
        for (int i = n - 1; i > 0; --i) {
            state = state * 1664525u + 1013904223u;
            int j = state % (i + 1);
            std::swap(arr[i], arr[j]);
        }
    }
    
public:
    explicit PerlinNoise(unsigned int seed = 0) {
        int p[256];
        for (int i = 0; i < 256; ++i) {
            p[i] = i;
        }
        shuffle(p, 256, seed);
        
        for (int i = 0; i < 512; ++i) {
            m_Permutation[i] = p[i & 255];
        }
    }
    
    // Сглаживание значения
    static float fade(float t) {
        return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    }
    
    // Линейная интерполяция
    static float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }
    
    // Градиент
    float grad(int hash, float x, float y, float z) const {
        int h = hash & 15;
        float u = h < 8 ? x : y;
        float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
    
    // 3D шум Перлина
    float noise(float x, float y, float z) const {
        int X = static_cast<int>(std::floor(x)) & 255;
        int Y = static_cast<int>(std::floor(y)) & 255;
        int Z = static_cast<int>(std::floor(z)) & 255;
        
        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);
        
        float u = fade(x);
        float v = fade(y);
        float w = fade(z);
        
        int A = m_Permutation[X] + Y;
        int AA = m_Permutation[A] + Z;
        int AB = m_Permutation[A + 1] + Z;
        int B = m_Permutation[X + 1] + Y;
        int BA = m_Permutation[B] + Z;
        int BB = m_Permutation[B + 1] + Z;
        
        return lerp(
            lerp(lerp(grad(m_Permutation[AA], x, y, z),
                     grad(m_Permutation[BA], x - 1.0f, y, z), u),
                lerp(grad(m_Permutation[AB], x, y - 1.0f, z),
                     grad(m_Permutation[BB], x - 1.0f, y - 1.0f, z), u), v),
            lerp(lerp(grad(m_Permutation[AA + 1], x, y, z - 1.0f),
                     grad(m_Permutation[BA + 1], x - 1.0f, y, z - 1.0f), u),
                lerp(grad(m_Permutation[AB + 1], x, y - 1.0f, z - 1.0f),
                     grad(m_Permutation[BB + 1], x - 1.0f, y - 1.0f, z - 1.0f), u), v),
            w
        );
    }
    
    // Фрактальный шум (октавы)
    float octaveNoise(float x, float y, float z, int octaves, float persistence = 0.5f) const {
        float total = 0.0f;
        float frequency = 1.0f;
        float amplitude = 1.0f;
        float maxValue = 0.0f;
        
        for (int i = 0; i < octaves; ++i) {
            total += noise(x * frequency, y * frequency, z * frequency) * amplitude;
            maxValue += amplitude;
            amplitude *= persistence;
            frequency *= 2.0f;
        }
        
        return total / maxValue;
    }
};

} // namespace EcoSystem
