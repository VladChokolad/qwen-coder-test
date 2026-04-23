#include "World.h"
#include "Noise.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace EcoSystem {

World::World(int width, int height, int depth, float cellSize, unsigned int seed)
    : m_Width(width)
    , m_Height(height)
    , m_Depth(depth)
    , m_CellSize(cellSize)
    , m_Seed(seed)
{
    // Инициализация ячеек
    m_Cells.resize(static_cast<size_t>(m_Width) * m_Height * m_Depth);
    
    // Регистрация стандартных биомов
    registerDefaultBiomes();
}

void World::registerDefaultBiomes() {
    // Луга
    BiomeParams grassland;
    grassland.type = BiomeType::Grassland;
    grassland.name = "Grassland";
    grassland.baseTemperature = 20.0f;
    grassland.humidity = 0.5f;
    grassland.vegetationDensity = 0.7f;
    grassland.foodAvailability = 0.7f;
    grassland.colorR = 0.3f;
    grassland.colorG = 0.7f;
    grassland.colorB = 0.2f;
    registerBiome(grassland);
    
    // Лес
    BiomeParams forest;
    forest.type = BiomeType::Forest;
    forest.name = "Forest";
    forest.baseTemperature = 18.0f;
    forest.humidity = 0.7f;
    forest.vegetationDensity = 0.9f;
    forest.foodAvailability = 0.6f;
    forest.colorR = 0.1f;
    forest.colorG = 0.5f;
    forest.colorB = 0.1f;
    registerBiome(forest);
    
    // Пустыня
    BiomeParams desert;
    desert.type = BiomeType::Desert;
    desert.name = "Desert";
    desert.baseTemperature = 35.0f;
    desert.temperatureVariation = 15.0f;
    desert.humidity = 0.1f;
    desert.vegetationDensity = 0.1f;
    desert.foodAvailability = 0.2f;
    desert.colorR = 0.9f;
    desert.colorG = 0.8f;
    desert.colorB = 0.5f;
    registerBiome(desert);
    
    // Тундра
    BiomeParams tundra;
    tundra.type = BiomeType::Tundra;
    tundra.name = "Tundra";
    tundra.baseTemperature = -10.0f;
    tundra.humidity = 0.3f;
    tundra.vegetationDensity = 0.2f;
    tundra.foodAvailability = 0.3f;
    tundra.colorR = 0.8f;
    tundra.colorG = 0.8f;
    tundra.colorB = 0.7f;
    registerBiome(tundra);
    
    // Горы
    BiomeParams mountain;
    mountain.type = BiomeType::Mountain;
    mountain.name = "Mountain";
    mountain.baseTemperature = 5.0f;
    mountain.humidity = 0.4f;
    mountain.vegetationDensity = 0.2f;
    mountain.foodAvailability = 0.3f;
    mountain.isWalkable = false;  // Горы труднопроходимы
    mountain.colorR = 0.5f;
    mountain.colorG = 0.5f;
    mountain.colorB = 0.5f;
    registerBiome(mountain);
    
    // Болото
    BiomeParams swamp;
    swamp.type = BiomeType::Swamp;
    swamp.name = "Swamp";
    swamp.baseTemperature = 22.0f;
    swamp.humidity = 0.9f;
    swamp.vegetationDensity = 0.6f;
    swamp.foodAvailability = 0.5f;
    swamp.colorR = 0.3f;
    swamp.colorG = 0.4f;
    swamp.colorB = 0.2f;
    registerBiome(swamp);
    
    // Тропический лес
    BiomeParams rainforest;
    rainforest.type = BiomeType::Rainforest;
    rainforest.name = "Rainforest";
    rainforest.baseTemperature = 28.0f;
    rainforest.humidity = 0.95f;
    rainforest.vegetationDensity = 1.0f;
    rainforest.foodAvailability = 0.9f;
    rainforest.colorR = 0.0f;
    rainforest.colorG = 0.6f;
    rainforest.colorB = 0.1f;
    registerBiome(rainforest);
    
    // Саванна
    BiomeParams savanna;
    savanna.type = BiomeType::Savanna;
    savanna.name = "Savanna";
    savanna.baseTemperature = 28.0f;
    savanna.humidity = 0.3f;
    savanna.vegetationDensity = 0.4f;
    savanna.foodAvailability = 0.5f;
    savanna.colorR = 0.8f;
    savanna.colorG = 0.7f;
    savanna.colorB = 0.3f;
    registerBiome(savanna);
}

void World::registerBiome(const BiomeParams& params) {
    // Проверяем, есть ли уже такой биом
    for (auto& biome : m_BiomeDefinitions) {
        if (biome.type == params.type) {
            biome = params;
            return;
        }
    }
    m_BiomeDefinitions.push_back(params);
}

void World::generate(unsigned int seed) {
    if (seed == 0) {
        seed = m_Seed;
    }
    m_Seed = seed;
    
    generateHeightmap();
    generateBiomes();
}

void World::generateHeightmap() {
    PerlinNoise noise(m_Seed);
    
    float minHeight = std::numeric_limits<float>::max();
    float maxHeight = std::numeric_limits<float>::lowest();
    
    // Генерация высот для каждой ячейки
    for (int z = 0; z < m_Depth; ++z) {
        for (int y = 0; y < m_Height; ++y) {
            for (int x = 0; x < m_Width; ++x) {
                WorldCell& cell = getCell(x, y, z);
                cell.position = Vector3(
                    static_cast<float>(x) * m_CellSize,
                    static_cast<float>(y) * m_CellSize,
                    static_cast<float>(z) * m_CellSize
                );
                
                // Используем фрактальный шум для более естественного ландшафта
                float nx = static_cast<float>(x) / static_cast<float>(m_Width) * 3.0f;
                float ny = static_cast<float>(y) / static_cast<float>(m_Height) * 3.0f;
                float nz = static_cast<float>(z) / static_cast<float>(m_Depth) * 3.0f;
                
                float heightValue = noise.octaveNoise(nx, ny, nz, 4, 0.5f);
                
                // Нормализация высоты от -1 до 1
                heightValue = std::clamp(heightValue, -1.0f, 1.0f);
                
                // Преобразование в абсолютную высоту (0-100 единиц)
                cell.height = (heightValue + 1.0f) * 50.0f;
                
                minHeight = std::min(minHeight, cell.height);
                maxHeight = std::max(maxHeight, cell.height);
            }
        }
    }
    
    // Нормализация высот к диапазону 0-100
    float heightRange = maxHeight - minHeight;
    if (heightRange > 0.001f) {
        for (auto& cell : m_Cells) {
            cell.height = (cell.height - minHeight) / heightRange * 100.0f;
        }
    }
}

void World::generateBiomes() {
    PerlinNoise tempNoise(m_Seed + 1000);
    PerlinNoise humidityNoise(m_Seed + 2000);
    
    for (auto& cell : m_Cells) {
        // Температура зависит от высоты и шума
        float baseTemp = 25.0f - cell.height * 0.2f;  // Чем выше, тем холоднее
        float tempVariation = tempNoise.octaveNoise(
            cell.position.x * 0.05f,
            cell.position.y * 0.05f,
            cell.position.z * 0.05f,
            2, 0.5f
        ) * 10.0f;
        cell.temperature = baseTemp + tempVariation;
        
        // Влажность
        cell.humidity = std::clamp(
            humidityNoise.octaveNoise(
                cell.position.x * 0.03f,
                cell.position.y * 0.03f,
                cell.position.z * 0.03f,
                2, 0.5f
            ),
            0.0f, 1.0f
        );
        
        // Растительность зависит от температуры и влажности
        float optimalTemp = std::abs(cell.temperature - 20.0f);
        float tempFactor = 1.0f - std::min(optimalTemp / 30.0f, 1.0f);
        cell.vegetation = std::clamp((cell.humidity * 0.7f + tempFactor * 0.3f), 0.0f, 1.0f);
        
        // Определение биома на основе параметров
        cell.biome = determineBiome(cell.height, cell.temperature, cell.humidity);
        
        // Проверка проходимости
        cell.isWalkable = (cell.biome != BiomeType::Mountain);
    }
}

BiomeType World::determineBiome(float height, float temperature, float humidity) const {
    // Горы (высоко)
    if (height > 75.0f) {
        return BiomeType::Mountain;
    }
    
    // Пустыня (жарко и сухо)
    if (temperature > 30.0f && humidity < 0.3f) {
        return BiomeType::Desert;
    }
    
    // Саванна (жарко и умеренно влажно)
    if (temperature > 25.0f && humidity >= 0.3f && humidity < 0.5f) {
        return BiomeType::Savanna;
    }
    
    // Тропический лес (жарко и очень влажно)
    if (temperature > 25.0f && humidity > 0.8f) {
        return BiomeType::Rainforest;
    }
    
    // Тундра (холодно)
    if (temperature < 5.0f) {
        return BiomeType::Tundra;
    }
    
    // Болото (умеренно тепло и очень влажно)
    if (temperature >= 15.0f && temperature <= 25.0f && humidity > 0.8f) {
        return BiomeType::Swamp;
    }
    
    // Лес (умеренно и влажно)
    if (humidity > 0.6f) {
        return BiomeType::Forest;
    }
    
    // По умолчанию - луга
    return BiomeType::Grassland;
}

const WorldCell& World::getCell(int x, int y, int z) const {
    if (!isValidCell(x, y, z)) {
        throw std::out_of_range("Cell coordinates out of range");
    }
    return m_Cells[coordsToIndex(x, y, z)];
}

WorldCell& World::getCell(int x, int y, int z) {
    if (!isValidCell(x, y, z)) {
        throw std::out_of_range("Cell coordinates out of range");
    }
    return m_Cells[coordsToIndex(x, y, z)];
}

const WorldCell& World::getCellAtPosition(const Vector3& position) const {
    int x = static_cast<int>(position.x / m_CellSize);
    int y = static_cast<int>(position.y / m_CellSize);
    int z = static_cast<int>(position.z / m_CellSize);
    
    if (!isValidCell(x, y, z)) {
        // Возвращаем первую ячейку если позиция вне границ
        return m_Cells[0];
    }
    return getCell(x, y, z);
}

bool World::isValidCell(int x, int y, int z) const {
    return x >= 0 && x < m_Width &&
           y >= 0 && y < m_Height &&
           z >= 0 && z < m_Depth;
}

int World::coordsToIndex(int x, int y, int z) const {
    return x + y * m_Width + z * m_Width * m_Height;
}

Vector3 World::indexToCoords(int index) const {
    int x = index % m_Width;
    int y = (index / m_Width) % m_Height;
    int z = index / (m_Width * m_Height);
    return Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
}

Vector3 World::getRandomWalkablePosition() const {
    std::vector<Vector3> walkablePositions;
    
    for (const auto& cell : m_Cells) {
        if (cell.isWalkable) {
            walkablePositions.push_back(cell.position);
        }
    }
    
    if (walkablePositions.empty()) {
        return Vector3(0.0f, 0.0f, 0.0f);
    }
    
    int randomIndex = rand() % walkablePositions.size();
    return walkablePositions[randomIndex];
}

std::vector<WorldCell> World::getNeighbors(int x, int y, int z, int radius) const {
    std::vector<WorldCell> neighbors;
    
    for (int dz = -radius; dz <= radius; ++dz) {
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                if (dx == 0 && dy == 0 && dz == 0) continue;
                
                int nx = x + dx;
                int ny = y + dy;
                int nz = z + dz;
                
                if (isValidCell(nx, ny, nz)) {
                    neighbors.push_back(getCell(nx, ny, nz));
                }
            }
        }
    }
    
    return neighbors;
}

bool World::isWalkable(const Vector3& position) const {
    try {
        const WorldCell& cell = getCellAtPosition(position);
        return cell.isWalkable;
    } catch (...) {
        return false;
    }
}

float World::getTemperature(const Vector3& position) const {
    try {
        const WorldCell& cell = getCellAtPosition(position);
        return cell.temperature;
    } catch (...) {
        return 20.0f;
    }
}

float World::getFoodAvailability(const Vector3& position) const {
    try {
        const WorldCell& cell = getCellAtPosition(position);
        
        // Поиск параметров биома
        for (const auto& biome : m_BiomeDefinitions) {
            if (biome.type == cell.biome) {
                return biome.foodAvailability * cell.vegetation;
            }
        }
        
        return cell.vegetation * 0.5f;
    } catch (...) {
        return 0.5f;
    }
}

void World::clear() {
    for (auto& cell : m_Cells) {
        cell = WorldCell();
    }
}

} // namespace EcoSystem
