#pragma once

#include <vector>
#include <random>
#include <chrono>

namespace EcoSystem {

// Генератор случайных чисел для симуляции
class RandomGenerator {
private:
    std::mt19937 m_RNG;
    std::uniform_real_distribution<float> m_FloatDist;
    std::uniform_int_distribution<int> m_IntDist;
    
public:
    RandomGenerator(unsigned int seed = 0) {
        if (seed == 0) {
            // Используем текущее время как seed
            auto now = std::chrono::high_resolution_clock::now();
            seed = static_cast<unsigned int>(
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    now.time_since_epoch()
                ).count()
            );
        }
        m_RNG.seed(seed);
        m_FloatDist = std::uniform_real_distribution<float>(0.0f, 1.0f);
        m_IntDist = std::uniform_int_distribution<int>(0, 100);
    }
    
    // Случайное float число в диапазоне [0, 1]
    float nextFloat() {
        return m_FloatDist(m_RNG);
    }
    
    // Случайное float число в диапазоне [min, max]
    float nextFloat(float min, float max) {
        return min + (max - min) * nextFloat();
    }
    
    // Случайное int число в диапазоне [min, max]
    int nextInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(m_RNG);
    }
    
    // Случайный элемент из вектора
    template<typename T>
    T& randomElement(std::vector<T>& vec) {
        if (vec.empty()) {
            throw std::runtime_error("Cannot get random element from empty vector");
        }
        std::uniform_int_distribution<size_t> dist(0, vec.size() - 1);
        return vec[dist(m_RNG)];
    }
    
    template<typename T>
    const T& randomElement(const std::vector<T>& vec) {
        if (vec.empty()) {
            throw std::runtime_error("Cannot get random element from empty vector");
        }
        std::uniform_int_distribution<size_t> dist(0, vec.size() - 1);
        return vec[dist(m_RNG)];
    }
    
    // Шанс события (возвращает true с вероятностью probability)
    bool chance(float probability) {
        return nextFloat() < probability;
    }
    
    // Перезапуск генератора с новым seed
    void reseed(unsigned int seed) {
        m_RNG.seed(seed);
    }
};

} // namespace EcoSystem
