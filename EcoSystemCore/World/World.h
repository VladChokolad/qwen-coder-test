#pragma once

#include "Types.h"
#include "Vector3.h"
#include <vector>
#include <string>
#include <functional>

namespace EcoSystem {

// Типы биомов
enum class BiomeType {
    Grassland,      // Луга
    Forest,         // Лес
    Desert,         // Пустыня
    Tundra,         // Тундра
    Mountain,       // Горы
    Swamp,          // Болото
    Rainforest,     // Тропический лес
    Savanna         // Саванна
};

// Параметры биома
struct BiomeParams {
    BiomeType type;
    std::string name;
    
    // Климат
    float baseTemperature = 20.0f;
    float temperatureVariation = 5.0f;
    float humidity = 0.5f;
    float rainfall = 0.5f;
    
    // Растительность
    float vegetationDensity = 0.5f;  // Количество растительности (0-1)
    float foodAvailability = 0.5f;   // Доступность пищи для травоядных
    
    // Цвет для визуализации (RGB)
    float colorR = 0.5f;
    float colorG = 0.5f;
    float colorB = 0.5f;
    
    // Проходимость
    bool isWalkable = true;
};

// Данные о ячейке мира
struct WorldCell {
    Vector3 position;
    float height;
    BiomeType biome;
    float temperature;
    float humidity;
    float vegetation;
    bool isWalkable;
    
    WorldCell() 
        : position(0.0f, 0.0f, 0.0f)
        , height(0.0f)
        , biome(BiomeType::Grassland)
        , temperature(20.0f)
        , humidity(0.5f)
        , vegetation(0.5f)
        , isWalkable(true) {}
};

// Класс мира симуляции
class World {
private:
    int m_Width;
    int m_Height;
    int m_Depth;
    float m_CellSize;
    
    std::vector<WorldCell> m_Cells;
    std::vector<BiomeParams> m_BiomeDefinitions;
    
    unsigned int m_Seed;
    
public:
    World(int width, int height, int depth, float cellSize = 1.0f, unsigned int seed = 0);
    virtual ~World() = default;
    
    // === Геттеры ===
    int getWidth() const { return m_Width; }
    int getHeight() const { return m_Height; }
    int getDepth() const { return m_Depth; }
    float getCellSize() const { return m_CellSize; }
    unsigned int getSeed() const { return m_Seed; }
    
    // === Методы доступа к ячейкам ===
    
    // Получение ячейки по координатам
    const WorldCell& getCell(int x, int y, int z = 0) const;
    WorldCell& getCell(int x, int y, int z = 0);
    
    // Получение ячейки по позиции в мире
    const WorldCell& getCellAtPosition(const Vector3& position) const;
    
    // Проверка существования ячейки
    bool isValidCell(int x, int y, int z = 0) const;
    
    // === Генерация мира ===
    
    // Процедурная генерация мира
    void generate(unsigned int seed = 0);
    
    // Генерация высот (использует шум Перлина или аналог)
    void generateHeightmap();
    
    // Генерация биомов на основе высоты и других факторов
    void generateBiomes();
    
    // Определение биома на основе параметров
    BiomeType determineBiome(float height, float temperature, float humidity) const;
    
    // Регистрация параметров биома
    void registerBiome(const BiomeParams& params);
    
    // Регистрация стандартных биомов
    void registerDefaultBiomes();
    
    // === Вспомогательные методы ===
    
    // Преобразование координат в индекс массива
    int coordsToIndex(int x, int y, int z = 0) const;
    
    // Преобразование индекса в координаты
    Vector3 indexToCoords(int index) const;
    
    // Получение случайной проходимой позиции
    Vector3 getRandomWalkablePosition() const;
    
    // Получение соседей ячейки
    std::vector<WorldCell> getNeighbors(int x, int y, int z = 0, int radius = 1) const;
    
    // Проверка проходимости позиции
    bool isWalkable(const Vector3& position) const;
    
    // Получение температуры в точке
    float getTemperature(const Vector3& position) const;
    
    // Получение доступности пищи в точке
    float getFoodAvailability(const Vector3& position) const;
    
    // Очистка мира
    void clear();
};

} // namespace EcoSystem
