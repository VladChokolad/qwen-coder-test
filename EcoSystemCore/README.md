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

## Сборка и запуск

### Требования

- **Операционная система**: Windows 10/11, Linux (Ubuntu 20.04+), macOS 11+
- **CMake**: версия 3.20 или выше
- **Компилятор C++17**:
  - Windows: MSVC 2019 или новее (входит в Visual Studio 2019+)
  - Linux: GCC 8+ или Clang 7+
  - macOS: Apple Clang 12+ или GCC 8+
- **Git**: для клонирования репозитория

### Пошаговая инструкция запуска

#### Шаг 1: Клонирование или подготовка проекта

Убедитесь, что вы находитесь в директории проекта `EcoSystemCore`:

```bash
cd EcoSystemCore
```

#### Шаг 2: Создание директории сборки

Создайте отдельную папку для файлов сборки (out-of-source build):

```bash
mkdir build
cd build
```

#### Шаг 3: Конфигурация проекта через CMake

Запустите CMake для генерации файлов сборки:

**Для Linux/macOS:**
```bash
cmake ..
```

**Для Windows (Visual Studio 2019+):**
```bash
# Генерация решения Visual Studio
cmake .. -G "Visual Studio 16 2019"
# или для VS 2022
cmake .. -G "Visual Studio 17 2022"
```

**Для Windows (MinGW):**
```bash
cmake .. -G "MinGW Makefiles"
```

**Опционально: указание типа сборки**
```bash
# Release сборка (оптимизированная)
cmake .. -DCMAKE_BUILD_TYPE=Release

# Debug сборка (с отладочной информацией)
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

#### Шаг 4: Компиляция проекта

**Для Linux/macOS (Make):**
```bash
make -j$(nproc)
```

**Для Windows (Visual Studio):**
```bash
# Из директории build запустите:
cmake --build . --config Release
```

**Для Windows (MinGW):**
```bash
mingw32-make -j4
```

#### Шаг 5: Запуск демонстрационной программы

После успешной компиляции запустите исполняемый файл:

**Для Linux/macOS:**
```bash
./EcoSystemDemo
```

**Для Windows:**
```bash
# Из директории build
Release\EcoSystemDemo.exe
# или
.\Release\EcoSystemDemo.exe
```

### Полный алгоритм запуска (одной командой)

**Linux/macOS:**
```bash
cd EcoSystemCore && \
mkdir -p build && \
cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) && \
./EcoSystemDemo
```

**Windows (PowerShell):**
```powershell
cd EcoSystemCore; `
mkdir -p build; `
cd build; `
cmake .. -DCMAKE_BUILD_TYPE=Release; `
cmake --build . --config Release; `
.\Release\EcoSystemDemo.exe
```

### Проверка успешности запуска

При успешном запуске вы увидите:

```
========================================
  EcoSystem Core Demo
========================================
World generated: 100x100
Biomes: Grassland, Forest, Desert, Tundra, Mountain, Swamp, Rainforest, Savanna

Spawning creatures...
  - Rabbits: 50
  - Wolves: 10
  - Bears: 5
  - Vultures: 3
  - Beetles: 30

Simulation started!
Press: [P] Pause/Resume, [+] Speed Up, [-] Slow Down, [Q] Quit

--- Tick 1 ---
Population: 98
  Herbivores: 80
  Carnivores: 13
  Omnivores: 5
Events: 2 births, 0 deaths
```

### Управление в демо-приложении

| Клавиша | Действие |
|---------|----------|
| `P` | Пауза / Продолжить симуляцию |
| `+` / `=` | Ускорить время (x2, x4, x8...) |
| `-` | Замедлить время |
| `R` | Сбросить скорость времени к 1.0 |
| `S` | Показать статистику |
| `L` | Показать последние события |
| `Q` | Выход из программы |

### Возможные проблемы и решения

#### Ошибка: "CMake not found"

**Решение:** Установите CMake:
- Ubuntu/Debian: `sudo apt-get install cmake`
- Fedora: `sudo dnf install cmake`
- macOS: `brew install cmake`
- Windows: скачайте с [cmake.org](https://cmake.org/download/)

#### Ошибка: "Compiler does not support C++17"

**Решение:** Обновите компилятор:
- Ubuntu: `sudo apt-get install g++-9` или новее
- macOS: обновите Xcode Command Line Tools
- Windows: установите Visual Studio 2019 или новее

#### Ошибка: "Build directory not found"

**Решение:** Убедитесь, что вы создали папку `build` и находитесь в ней перед запуском `cmake`.

#### Ошибка при запуске: "Executable not found"

**Решение:** Проверьте, что компиляция прошла успешно. Исполняемый файл находится в папке `build/` (Linux/macOS) или `build/Release/` (Windows).

### Интеграция в свой проект

Если вы хотите использовать EcoSystem Core как библиотеку:

1. Добавьте `EcoSystemCore` как подмодуль или скопируйте исходники
2. В вашем `CMakeLists.txt`:
   ```cmake
   add_subdirectory(EcoSystemCore)
   target_link_libraries(YourProject EcoSystemCore)
   ```
3. Подключайте заголовки: `#include "Simulation.h"`

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
