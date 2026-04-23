#pragma once

#include <string>
#include <functional>

namespace EcoSystem {

// Типы существ
enum class CreatureType {
    Herbivore,      // Травоядные
    Carnivore,      // Хищники
    Omnivore,       // Всеядные
    Scavenger,      // Падальщики
    Insect,         // Насекомые
    Bird,           // Птицы
    Fish,           // Рыбы
    Amphibian,      // Амфибии
    Reptile         // Рептилии
};

// Состояния существа
enum class CreatureState {
    Idle,           // Бездействие
    Moving,         // Движение
    Eating,         // Питание
    Hunting,        // Охота
    Fleeing,        // Бегство
    Mating,         // Размножение
    Resting,        // Отдых
    Dead            // Мертво
};

// Структура параметров вида
struct SpeciesParams {
    std::string name;
    CreatureType type;
    
    // Физические параметры
    float baseHealth = 100.0f;
    float maxHealth = 100.0f;
    float moveSpeed = 1.0f;
    float perceptionRadius = 10.0f;
    float attackDamage = 10.0f;
    float attackRange = 1.5f;
    
    // Потребности
    float hungerRate = 0.5f;        // Скорость голода в секунду
    float maxHunger = 100.0f;
    float energyRate = 0.3f;        // Скорость расхода энергии
    float maxEnergy = 100.0f;
    
    // Размножение
    float reproductionThreshold = 80.0f;  // Мин. энергия для размножения
    float reproductionCooldown = 30.0f;   // Перезарядка размножения
    int offspringCount = 2;               // Количество потомков
    
    // Поведение
    float aggression = 0.5f;        // Агрессивность (0-1)
    float sociability = 0.3f;       // Социальность (0-1)
    float fearFactor = 0.5f;        // Коэффициент страха
    float preferredTemperature = 20.0f;
    float temperatureTolerance = 10.0f;
    
    // Пищевая ценность
    float foodValue = 50.0f;        // Сколько энергии дает при поедании
};

} // namespace EcoSystem
