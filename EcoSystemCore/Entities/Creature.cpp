#include "Creature.h"
#include <cmath>
#include <algorithm>

namespace EcoSystem {

Creature::Creature(const SpeciesParams& species)
    : m_ID(EntityID::Generate())
    , m_Species(species)
    , m_State(CreatureState::Idle)
    , m_Position(0.0f, 0.0f, 0.0f)
    , m_Velocity(0.0f, 0.0f, 0.0f)
    , m_Rotation(0.0f)
    , m_CurrentHealth(species.baseHealth)
    , m_CurrentHunger(0.0f)
    , m_CurrentEnergy(species.maxEnergy)
    , m_Age(0.0f)
    , m_ReproductionCooldown(0.0f)
    , m_FoodConsumed(0)
    , m_OffspringCount(0)
    , m_DistanceTraveled(0.0f)
    , m_IsDead(false)
{
}

void Creature::update(float deltaTime) {
    if (m_IsDead) {
        return;
    }
    
    // Обновление возраста
    m_Age += deltaTime;
    
    // Обновление перезарядки размножения
    if (m_ReproductionCooldown > 0.0f) {
        m_ReproductionCooldown -= deltaTime;
    }
    
    // Увеличение голода
    m_CurrentHunger += m_Species.hungerRate * deltaTime;
    if (m_CurrentHunger > m_Species.maxHunger) {
        m_CurrentHunger = m_Species.maxHunger;
    }
    
    // Расход энергии
    m_CurrentEnergy -= m_Species.energyRate * deltaTime;
    
    // Если существо движется, тратится больше энергии
    if (m_State == CreatureState::Moving || 
        m_State == CreatureState::Hunting || 
        m_State == CreatureState::Fleeing) {
        m_CurrentEnergy -= m_Species.energyRate * deltaTime * 0.5f;
    }
    
    // Ограничение энергии
    if (m_CurrentEnergy < 0.0f) {
        m_CurrentEnergy = 0.0f;
    }
    
    // Голод влияет на здоровье
    if (m_CurrentHunger >= m_Species.maxHunger * 0.8f) {
        // Сильный голод - потеря здоровья
        float hungerDamage = (m_CurrentHunger / m_Species.maxHunger - 0.8f) * deltaTime * 5.0f;
        m_CurrentHealth -= hungerDamage;
    }
    
    // Проверка смерти
    if (m_CurrentHealth <= 0.0f) {
        die();
    }
    
    // Обновление позиции на основе скорости
    if (m_Velocity.lengthSquared() > 0.0001f) {
        m_Position += m_Velocity * deltaTime;
        m_DistanceTraveled += m_Velocity.length() * deltaTime;
        
        // Поворот в направлении движения
        m_Rotation = std::atan2(m_Velocity.x, m_Velocity.z);
    }
}

void Creature::takeDamage(float damage) {
    if (m_IsDead) {
        return;
    }
    
    m_CurrentHealth -= damage;
    if (m_CurrentHealth <= 0.0f) {
        die();
    }
}

void Creature::heal(float amount) {
    if (m_IsDead) {
        return;
    }
    
    m_CurrentHealth += amount;
    if (m_CurrentHealth > m_Species.maxHealth) {
        m_CurrentHealth = m_Species.maxHealth;
    }
}

void Creature::feed(float foodValue) {
    if (m_IsDead) {
        return;
    }
    
    m_FoodConsumed++;
    
    // Восстановление энергии от еды
    m_CurrentEnergy += foodValue * 0.7f;
    if (m_CurrentEnergy > m_Species.maxEnergy) {
        m_CurrentEnergy = m_Species.maxEnergy;
    }
    
    // Уменьшение голода
    m_CurrentHunger -= foodValue * 0.5f;
    if (m_CurrentHunger < 0.0f) {
        m_CurrentHunger = 0.0f;
    }
}

void Creature::die() {
    if (m_IsDead) {
        return;
    }
    
    m_IsDead = true;
    m_State = CreatureState::Dead;
    m_Velocity = Vector3(0.0f, 0.0f, 0.0f);
}

std::unique_ptr<Creature> Creature::reproduce() {
    if (m_IsDead) {
        return nullptr;
    }
    
    // Проверка условий для размножения
    if (m_CurrentEnergy < m_Species.reproductionThreshold) {
        return nullptr;
    }
    
    if (m_ReproductionCooldown > 0.0f) {
        return nullptr;
    }
    
    // Размножение стоит энергии
    float reproductionCost = m_Species.reproductionThreshold * 0.5f;
    m_CurrentEnergy -= reproductionCost;
    m_ReproductionCooldown = m_Species.reproductionCooldown;
    m_OffspringCount++;
    
    // Создание потомка с небольшими мутациями
    SpeciesParams offspringParams = m_Species;
    
    // Небольшие случайные изменения параметров (мутации)
    const float mutationRate = 0.1f;
    offspringParams.moveSpeed *= (1.0f + (static_cast<float>(rand()) / RAND_MAX - 0.5f) * mutationRate);
    offspringParams.perceptionRadius *= (1.0f + (static_cast<float>(rand()) / RAND_MAX - 0.5f) * mutationRate);
    offspringParams.maxHealth *= (1.0f + (static_cast<float>(rand()) / RAND_MAX - 0.5f) * mutationRate);
    
    // Ограничение значений
    offspringParams.moveSpeed = std::max(0.1f, offspringParams.moveSpeed);
    offspringParams.perceptionRadius = std::max(1.0f, offspringParams.perceptionRadius);
    offspringParams.maxHealth = std::max(10.0f, offspringParams.maxHealth);
    
    auto offspring = std::make_unique<Creature>(offspringParams);
    offspring->setPosition(m_Position);
    
    return offspring;
}

void Creature::moveTowards(const Vector3& target, float deltaTime) {
    if (m_IsDead) {
        return;
    }
    
    Vector3 direction = target - m_Position;
    float distance = direction.length();
    
    if (distance > 0.01f) {
        direction = direction.normalized();
        m_Velocity = direction * m_Species.moveSpeed;
        m_State = CreatureState::Moving;
        
        // Перемещение
        float moveDistance = std::min(distance, m_Species.moveSpeed * deltaTime);
        m_Position += direction * moveDistance;
        m_DistanceTraveled += moveDistance;
        
        // Поворот в направлении движения
        m_Rotation = std::atan2(direction.x, direction.z);
    }
}

void Creature::wander(float deltaTime) {
    if (m_IsDead) {
        return;
    }
    
    // Случайное изменение направления
    float angleChange = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f;
    m_Rotation += angleChange * deltaTime;
    
    // Движение в текущем направлении
    float cosRot = std::cos(m_Rotation);
    float sinRot = std::sin(m_Rotation);
    m_Velocity = Vector3(sinRot * m_Species.moveSpeed * 0.5f, 0.0f, cosRot * m_Species.moveSpeed * 0.5f);
    
    m_Position += m_Velocity * deltaTime;
    m_DistanceTraveled += m_Velocity.length() * deltaTime;
    m_State = CreatureState::Idle;
}

bool Creature::canSee(const Vector3& targetPos) const {
    if (m_IsDead) {
        return false;
    }
    
    float distance = distanceTo(targetPos);
    return distance <= m_Species.perceptionRadius;
}

float Creature::distanceTo(const Vector3& point) const {
    return m_Position.distanceTo(point);
}

std::string Creature::getStateString() const {
    switch (m_State) {
        case CreatureState::Idle: return "Idle";
        case CreatureState::Moving: return "Moving";
        case CreatureState::Eating: return "Eating";
        case CreatureState::Hunting: return "Hunting";
        case CreatureState::Fleeing: return "Fleeing";
        case CreatureState::Mating: return "Mating";
        case CreatureState::Resting: return "Resting";
        case CreatureState::Dead: return "Dead";
        default: return "Unknown";
    }
}

void Creature::resetStats() {
    m_FoodConsumed = 0;
    m_OffspringCount = 0;
    m_DistanceTraveled = 0.0f;
}

} // namespace EcoSystem
