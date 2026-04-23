# EcoSystem Core

Ядро симуляции замкнутой экосистемы живых существ на C++. Этот модуль содержит всю логику симуляции и не зависит от движка визуализации (Unreal Engine 5).

## Архитектура проекта

```
EcoSystemCore/
├── Core/                    # Базовые компоненты
│   ├── Types.h             # Типы и перечисления
│   ├── Vector3.h           # 3D вектор
│   ├── EntityID.h          # Уникальные идентификаторы
│   └── RandomGenerator.h   # Генератор случайных чисел
│
├── Entities/               # Сущности
│   ├── Creature.h/cpp      # Базовый класс живого существа
│
├── World/                  # Мир симуляции
│   ├── World.h/cpp         # Класс мира
│   └── Noise.h             # Шум Перлина для генерации
│
├── Simulation/             # Логика симуляции
│   ├── Simulation.h/cpp    # Основной класс симуляции
│
├── examples/               # Примеры использования
│   └── main.cpp            # Демонстрационная программа
│
└── CMakeLists.txt          # Конфигурация сборки
```

## Основные компоненты

### 1. Creature (Существо)

Базовый класс всех живых существ в симуляции:

- **Типы**: Herbivore, Carnivore, Omnivore, Scavenger, Insect, Bird, Fish, Amphibian, Reptile
- **Состояния**: Idle, Moving, Eating, Hunting, Fleeing, Mating, Resting, Dead
- **Параметры**: здоровье, голод, энергия, скорость, восприятие, агрессия, социальность
- **Поведение**: поиск пищи, охота, бегство, размножение, блуждание

### 2. World (Мир)

Процедурная генерация мира:

- **Размер**: настраиваемая сетка X×Y×Z
- **Биомы**: Grassland, Forest, Desert, Tundra, Mountain, Swamp, Rainforest, Savanna
- **Генерация**: шум Перлина для высот, температуры, влажности
- **Ячейки**: высота, температура, влажность, растительность, проходимость

### 3. Simulation (Симуляция)

Управление всей экосистемой:

- **Контроль времени**: запуск, пауза, ускорение
- **Управление существами**: спавн, удаление, поиск
- **Взаимодействия**: хищник-жертва, поедание, размножение
- **События**: логирование, callback'и для внешних систем
- **Статистика**: популяция, рождения, смерти, средний возраст/здоровье

## Виды существ (по умолчанию)

| Вид | Тип | Здоровье | Скорость | Особенности |
|-----|-----|----------|----------|-------------|
| Rabbit | Herbivore | 50 | 3.0 | Быстро размножается, боится хищников |
| Wolf | Carnivore | 100 | 4.0 | Охотится на травоядных |
| Bear | Omnivore | 150 | 2.5 | Всеядный, сильный |
| Vulture | Scavenger | 40 | 5.0 | Поедает мертвых, большое восприятие |
| Beetle | Insect | 10 | 1.5 | Быстрое размножение, много потомков |

## Сборка

### Требования

- CMake 3.20+
- C++17 совместимый компилятор (GCC 8+, Clang 7+, MSVC 2019+)

### Компиляция

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
./EcoSystemDemo
```

## Использование

### Базовый пример

```cpp
#include "Simulation.h"

using namespace EcoSystem;

int main() {
    // Создание симуляции
    Simulation sim(100, 100, 1);
    
    // Генерация мира
    sim.generateWorld(42);
    
    // Спавн существ
    SpeciesParams rabbitParams = createDefaultSpecies(CreatureType::Herbivore);
    sim.spawnCreatures(rabbitParams, 50);
    
    SpeciesParams wolfParams = createDefaultSpecies(CreatureType::Carnivore);
    sim.spawnCreatures(wolfParams, 10);
    
    // Запуск
    sim.start();
    
    // Главный цикл
    while (sim.isRunning()) {
        sim.update(deltaTime);
        
        const auto& stats = sim.getPopulationStats();
        std::cout << "Population: " << stats.totalCreatures << std::endl;
    }
    
    return 0;
}
```

### Расширение: добавление нового вида

```cpp
// Создание параметров нового вида
SpeciesParams dragonParams;
dragonParams.name = "Dragon";
dragonParams.type = CreatureType::Carnivore;
dragonParams.baseHealth = 500.0f;
dragonParams.maxHealth = 500.0f;
dragonParams.moveSpeed = 6.0f;
dragonParams.perceptionRadius = 50.0f;
dragonParams.attackDamage = 100.0f;
dragonParams.aggression = 0.9f;
dragonParams.foodValue = 200.0f;

// Спавн
sim.spawnCreatures(dragonParams, 2);
```

## Интеграция с Unreal Engine 5

Этот модуль разработан для независимости от движка. Для интеграции с UE5:

1. Скопируйте папку `EcoSystemCore` в проект UE5
2. Добавьте в Build.cs:
   ```csharp
   PublicIncludePaths.Add(ModuleDirectory + "/EcoSystemCore");
   ```
3. Используйте класс `Simulation` как источник данных
4. Подпишитесь на события через `setEventCallback()`
5. Визуализируйте существ через SpawnActor в UE5

### Разделение логики и визуализации

```
┌─────────────────┐     ┌──────────────────┐
│   Simulation    │────▶│  UE5 Visualizer  │
│   (EcoSystem    │     │  (Blueprints/    │
│    Core)        │     │   C++)           │
│                 │◀────│                  │
│ - Логика        │     │ - Рендеринг      │
│ - ИИ            │     │ - Анимации       │
│ - Физика        │     │ - Звук           │
└─────────────────┘     └──────────────────┘
```

## Производительность

- Оптимизировано для 100+ активных существ
- Пространственное разбиение для поиска соседей
- Минимальные аллокации в горячем цикле
- Поддержка multithreading (планируется)

## Лицензия

MIT License
