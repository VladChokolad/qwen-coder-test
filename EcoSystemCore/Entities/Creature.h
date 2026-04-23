#pragma once

#include "Types.h"
#include "Vector3.h"
#include "EntityID.h"
#include <memory>
#include <string>

namespace EcoSystem {

// Базовый класс живого существа
class Creature {
private:
    EntityID m_ID;
    SpeciesParams m_Species;
    
    // Текущее состояние
    CreatureState m_State;
    Vector3 m_Position;
    Vector3 m_Velocity;
    float m_Rotation;  // Угол поворота вокруг оси Y (в радианах)
    
    // Жизненные показатели
    float m_CurrentHealth;
    float m_CurrentHunger;   // 0 = сыт, maxHunger = голоден
    float m_CurrentEnergy;   // 0 = истощен, maxEnergy = полон сил
    float m_Age;             // Возраст в секундах симуляции
    float m_ReproductionCooldown;
    
    // Статистика
    int m_FoodConsumed;
    int m_OffspringCount;
    float m_DistanceTraveled;
    
    bool m_IsDead;
    
public:
    explicit Creature(const SpeciesParams& species);
    virtual ~Creature() = default;
    
    // === Геттеры ===
    EntityID getID() const { return m_ID; }
    const SpeciesParams& getSpecies() const { return m_Species; }
    CreatureType getType() const { return m_Species.type; }
    CreatureState getState() const { return m_State; }
    Vector3 getPosition() const { return m_Position; }
    Vector3 getVelocity() const { return m_Velocity; }
    float getRotation() const { return m_Rotation; }
    float getHealth() const { return m_CurrentHealth; }
    float getMaxHealth() const { return m_Species.maxHealth; }
    float getHunger() const { return m_CurrentHunger; }
    float getMaxHunger() const { return m_Species.maxHunger; }
    float getEnergy() const { return m_CurrentEnergy; }
    float getMaxEnergy() const { return m_Species.maxEnergy; }
    float getAge() const { return m_Age; }
    float getPerceptionRadius() const { return m_Species.perceptionRadius; }
    float getAttackDamage() const { return m_Species.attackDamage; }
    float getAttackRange() const { return m_Species.attackRange; }
    float getMoveSpeed() const { return m_Species.moveSpeed; }
    int getOffspringCount() const { return m_OffspringCount; }
    bool isDead() const { return m_IsDead; }
    
    // === Основные методы обновления ===
    
    // Обновление состояния существа (вызывается каждый кадр симуляции)
    virtual void update(float deltaTime);
    
    // Применение урона
    void takeDamage(float damage);
    
    // Лечение
    void heal(float amount);
    
    // Питание (восстанавливает энергию и голод)
    void feed(float foodValue);
    
    // Смерть существа
    void die();
    
    // Размножение (возвращает параметры потомка или nullptr если нельзя размножаться)
    std::unique_ptr<Creature> reproduce();
    
    // === Методы изменения состояния ===
    
    void setPosition(const Vector3& pos) { m_Position = pos; }
    void setVelocity(const Vector3& vel) { m_Velocity = vel; }
    void setRotation(float rot) { m_Rotation = rot; }
    void setState(CreatureState state) { m_State = state; }
    
    // Движение в направлении
    void moveTowards(const Vector3& target, float deltaTime);
    
    // Случайное блуждание
    void wander(float deltaTime);
    
    // === Вспомогательные методы ===
    
    // Проверка, может ли существо видеть другую позицию
    bool canSee(const Vector3& targetPos) const;
    
    // Расстояние до другой точки
    float distanceTo(const Vector3& point) const;
    
    // Получение строкового представления состояния
    std::string getStateString() const;
    
    // Сброс статистики
    void resetStats();
};

} // namespace EcoSystem
