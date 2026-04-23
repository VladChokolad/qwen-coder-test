#include "Simulation.h"
#include "SpeciesFactory.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>

namespace EcoSystem {

Simulation::Simulation(int worldWidth, int worldHeight, int worldDepth)
    : m_World(std::make_unique<World>(worldWidth, worldHeight, worldDepth))
    , m_TimeScale(1.0f)
    , m_SimulationTime(0.0f)
    , m_IsRunning(false)
    , m_IsPaused(false)
    , m_MaxEvents(100)
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

void Simulation::start() {
    m_IsRunning = true;
    m_IsPaused = false;
    logEvent(SimulationEvent::PopulationChange, EntityID(), "Simulation started");
}

void Simulation::stop() {
    m_IsRunning = false;
    m_IsPaused = false;
    logEvent(SimulationEvent::PopulationChange, EntityID(), "Simulation stopped");
}

void Simulation::pause() {
    if (m_IsRunning) {
        m_IsPaused = true;
        logEvent(SimulationEvent::PopulationChange, EntityID(), "Simulation paused");
    }
}

void Simulation::resume() {
    if (m_IsRunning && m_IsPaused) {
        m_IsPaused = false;
        logEvent(SimulationEvent::PopulationChange, EntityID(), "Simulation resumed");
    }
}

void Simulation::togglePause() {
    if (m_IsPaused) {
        resume();
    } else {
        pause();
    }
}

void Simulation::setTimeScale(float scale) {
    m_TimeScale = std::max(0.1f, std::min(10.0f, scale));
}

void Simulation::update(float deltaTime) {
    if (!m_IsRunning || m_IsPaused) {
        return;
    }
    
    // Применяем масштаб времени
    float scaledDelta = deltaTime * m_TimeScale;
    m_SimulationTime += scaledDelta;
    
    // Обновляем всех существ
    for (auto& creature : m_Creatures) {
        if (!creature->isDead()) {
            creature->update(scaledDelta);
        }
    }
    
    // Обработка взаимодействий
    processInteractions(scaledDelta);
    
    // Очистка мертвых существ
    cleanupDeadCreatures();
    
    // Обновление статистики
    updatePopulationStats();
}

void Simulation::generateWorld(unsigned int seed) {
    m_World->generate(seed);
    logEvent(SimulationEvent::PopulationChange, EntityID(), 
             "World generated with seed: " + std::to_string(seed));
}

void Simulation::addCreature(std::unique_ptr<Creature> creature) {
    if (creature) {
        // Размещаем существо в случайной проходимой позиции
        Vector3 pos = m_World->getRandomWalkablePosition();
        creature->setPosition(pos);
        
        m_Creatures.push_back(std::move(creature));
        logEvent(SimulationEvent::CreatureBorn, m_Creatures.back()->getID(), 
                 "Creature spawned: " + m_Creatures.back()->getSpecies().name);
    }
}

void Simulation::removeCreature(EntityID id) {
    auto it = std::find_if(m_Creatures.begin(), m_Creatures.end(),
                           [id](const std::unique_ptr<Creature>& c) {
                               return c->getID() == id;
                           });
    
    if (it != m_Creatures.end()) {
        m_Creatures.erase(it);
    }
}

void Simulation::cleanupDeadCreatures() {
    auto deadIt = std::remove_if(m_Creatures.begin(), m_Creatures.end(),
                                  [](const std::unique_ptr<Creature>& c) {
                                      return c->isDead();
                                  });
    
    int removedCount = static_cast<int>(std::distance(deadIt, m_Creatures.end()));
    if (removedCount > 0) {
        m_Stats.deaths += removedCount;
        logEvent(SimulationEvent::CreatureDied, EntityID(), 
                 "Cleaned up " + std::to_string(removedCount) + " dead creatures");
    }
    
    m_Creatures.erase(deadIt, m_Creatures.end());
}

Creature* Simulation::getCreature(EntityID id) {
    for (auto& creature : m_Creatures) {
        if (creature->getID() == id) {
            return creature.get();
        }
    }
    return nullptr;
}

const Creature* Simulation::getCreature(EntityID id) const {
    for (const auto& creature : m_Creatures) {
        if (creature->getID() == id) {
            return creature.get();
        }
    }
    return nullptr;
}

std::vector<Creature*> Simulation::getAllCreatures() {
    std::vector<Creature*> result;
    result.reserve(m_Creatures.size());
    for (auto& creature : m_Creatures) {
        result.push_back(creature.get());
    }
    return result;
}

std::vector<const Creature*> Simulation::getAllCreatures() const {
    std::vector<const Creature*> result;
    result.reserve(m_Creatures.size());
    for (const auto& creature : m_Creatures) {
        result.push_back(creature.get());
    }
    return result;
}

std::vector<Creature*> Simulation::getCreaturesInRadius(const Vector3& position, float radius) {
    std::vector<Creature*> result;
    
    for (auto& creature : m_Creatures) {
        if (!creature->isDead() && creature->distanceTo(position) <= radius) {
            result.push_back(creature.get());
        }
    }
    
    return result;
}

void Simulation::spawnRandomCreature() {
    // Выбираем случайный тип
    CreatureType types[] = {
        CreatureType::Herbivore,
        CreatureType::Carnivore,
        CreatureType::Omnivore,
        CreatureType::Scavenger,
        CreatureType::Insect
    };
    
    CreatureType randomType = types[rand() % 5];
    SpeciesParams params = createDefaultSpecies(randomType);
    
    auto creature = std::make_unique<Creature>(params);
    addCreature(std::move(creature));
}

void Simulation::spawnCreatures(const SpeciesParams& species, int count) {
    for (int i = 0; i < count; ++i) {
        auto creature = std::make_unique<Creature>(species);
        addCreature(std::move(creature));
    }
}

void Simulation::processInteractions(float deltaTime) {
    // Оптимизация: используем пространственное разбиение для больших популяций
    // Для простоты реализуем O(n²) проверку с ранним выходом
    
    std::vector<Creature*> aliveCreatures;
    for (auto& creature : m_Creatures) {
        if (!creature->isDead()) {
            aliveCreatures.push_back(creature.get());
        }
    }
    
    // Обработка поведения каждого существа
    for (Creature* creature : aliveCreatures) {
        if (creature->isDead()) continue;
        
        // Поиск ближайших существ
        std::vector<Creature*> nearby = getCreaturesInRadius(
            creature->getPosition(), 
            creature->getPerceptionRadius()
        );
        
        // Поведение в зависимости от типа
        switch (creature->getType()) {
            case CreatureType::Carnivore:
            case CreatureType::Omnivore: {
                // Хищники и всеядные ищут добычу
                Creature* prey = findNearestTarget(creature, CreatureType::Herbivore);
                if (prey && !prey->isDead()) {
                    creature->setState(CreatureState::Hunting);
                    creature->moveTowards(prey->getPosition(), deltaTime);
                    
                    // Атака если достаточно близко
                    if (creature->distanceTo(prey->getPosition()) <= creature->getAttackRange()) {
                        attack(creature, prey);
                    }
                } else {
                    // Если нет добычи, ищем пищу или блуждаем
                    creature->wander(deltaTime);
                }
                break;
            }
            
            case CreatureType::Herbivore: {
                // Травоядные ищут еду и убегают от хищников
                Creature* predator = findNearestTarget(creature, CreatureType::Carnivore);
                if (predator && !predator->isDead()) {
                    // Бегство от хищника
                    creature->setState(CreatureState::Fleeing);
                    Vector3 fleeDirection = creature->getPosition() - predator->getPosition();
                    fleeDirection = fleeDirection.normalized() * 10.0f;  // В противоположную сторону
                    creature->moveTowards(creature->getPosition() + fleeDirection, deltaTime);
                } else {
                    // Поиск пищи
                    creature->wander(deltaTime);
                }
                break;
            }
            
            case CreatureType::Scavenger: {
                // Падальщики ищут мертвых существ
                bool foundCorpse = false;
                for (const auto& c : m_Creatures) {
                    if (c->isDead() && creature->distanceTo(c->getPosition()) <= creature->getPerceptionRadius()) {
                        creature->setState(CreatureState::Eating);
                        creature->moveTowards(c->getPosition(), deltaTime);
                        if (creature->distanceTo(c->getPosition()) <= 1.0f) {
                            consume(creature, c->getSpecies().foodValue * 0.5f);
                            foundCorpse = true;
                        }
                        break;
                    }
                }
                if (!foundCorpse) {
                    creature->wander(deltaTime);
                }
                break;
            }
            
            default:
                creature->wander(deltaTime);
                break;
        }
        
        // Попытка размножения
        if (creature->getEnergy() >= creature->getSpecies().reproductionThreshold) {
            auto offspring = creature->reproduce();
            if (offspring) {
                addCreature(std::move(offspring));
                m_Stats.births++;
                logEvent(SimulationEvent::CreatureBorn, creature->getID(), 
                         "Creature reproduced");
            }
        }
    }
}

Creature* Simulation::findNearestTarget(Creature* creature, CreatureType preferredPrey) {
    Creature* nearest = nullptr;
    float nearestDist = std::numeric_limits<float>::max();
    
    for (auto& other : m_Creatures) {
        if (other.get() == creature || other->isDead()) continue;
        
        // Проверка типа цели
        bool isPreferred = (other->getType() == preferredPrey);
        bool isEdible = false;
        
        if (creature->getType() == CreatureType::Carnivore) {
            isEdible = (other->getType() == CreatureType::Herbivore || 
                       other->getType() == CreatureType::Insect);
        } else if (creature->getType() == CreatureType::Omnivore) {
            isEdible = (other->getType() == CreatureType::Herbivore || 
                       other->getType() == CreatureType::Insect);
        } else if (creature->getType() == CreatureType::Herbivore) {
            isEdible = (other->getType() == CreatureType::Carnivore);  // Для бегства
        }
        
        if (isEdible || isPreferred) {
            float dist = creature->distanceTo(other->getPosition());
            if (dist < nearestDist && dist <= creature->getPerceptionRadius()) {
                nearestDist = dist;
                nearest = other.get();
                
                // Если это предпочтительная цель, берем её сразу
                if (isPreferred) {
                    break;
                }
            }
        }
    }
    
    return nearest;
}

Vector3 Simulation::findFoodSource(const Vector3& position, float perceptionRadius) {
    // Для травоядных еда - это растительность в мире
    // Возвращаем направление к области с высокой растительностью
    
    float bestFoodValue = 0.0f;
    Vector3 bestPosition = position;
    
    // Проверяем несколько точек вокруг
    for (int i = 0; i < 8; ++i) {
        float angle = (static_cast<float>(i) / 8.0f) * 6.28f;
        Vector3 testPos(
            position.x + std::cos(angle) * perceptionRadius,
            position.y,
            position.z + std::sin(angle) * perceptionRadius
        );
        
        float foodValue = m_World->getFoodAvailability(testPos);
        if (foodValue > bestFoodValue) {
            bestFoodValue = foodValue;
            bestPosition = testPos;
        }
    }
    
    return bestPosition;
}

bool Simulation::attack(Creature* attacker, Creature* target) {
    if (!attacker || !target || attacker->isDead() || target->isDead()) {
        return false;
    }
    
    float damage = attacker->getAttackDamage();
    target->takeDamage(damage);
    
    logEvent(SimulationEvent::PredatorAttack, attacker->getID(),
             attacker->getSpecies().name + " attacked " + target->getSpecies().name + 
             " for " + std::to_string(static_cast<int>(damage)) + " damage");
    
    if (target->isDead()) {
        logEvent(SimulationEvent::CreatureDied, target->getID(),
                 target->getSpecies().name + " was killed by " + attacker->getSpecies().name);
        
        // Хищник может съесть добычу
        consume(attacker, target->getSpecies().foodValue);
        return true;
    }
    
    return false;
}

void Simulation::consume(Creature* creature, float foodValue) {
    if (!creature || creature->isDead()) {
        return;
    }
    
    creature->feed(foodValue);
    
    logEvent(SimulationEvent::CreatureAte, creature->getID(),
             creature->getSpecies().name + " consumed food worth " + 
             std::to_string(static_cast<int>(foodValue)));
}

void Simulation::logEvent(SimulationEvent type, EntityID creatureID, const std::string& description) {
    EventData event(type, creatureID, description, m_SimulationTime);
    m_Events.push_back(event);
    
    // Ограничиваем размер лога
    if (m_Events.size() > m_MaxEvents) {
        m_Events.erase(m_Events.begin());
    }
    
    // Вызов callback если установлен
    if (m_EventCallback) {
        m_EventCallback(event);
    }
    
    // Вывод в консоль для отладки
    #ifdef DEBUG
    std::cout << "[T=" << m_SimulationTime << "] " << description << std::endl;
    #endif
}

const std::vector<EventData>& Simulation::getRecentEvents(size_t count) const {
    // Возвращаем последние N событий
    static std::vector<EventData> recentEvents;
    recentEvents.clear();
    
    size_t startIdx = m_Events.size() > count ? m_Events.size() - count : 0;
    for (size_t i = startIdx; i < m_Events.size(); ++i) {
        recentEvents.push_back(m_Events[i]);
    }
    
    return recentEvents;
}

void Simulation::clearEvents() {
    m_Events.clear();
}

void Simulation::setEventCallback(std::function<void(const EventData&)> callback) {
    m_EventCallback = std::move(callback);
}

void Simulation::updatePopulationStats() {
    m_Stats.totalCreatures = static_cast<int>(m_Creatures.size());
    m_Stats.herbivores = 0;
    m_Stats.carnivores = 0;
    m_Stats.omnivores = 0;
    
    float totalAge = 0.0f;
    float totalHealth = 0.0f;
    
    for (const auto& creature : m_Creatures) {
        if (creature->isDead()) continue;
        
        switch (creature->getType()) {
            case CreatureType::Herbivore:
            case CreatureType::Insect:
                m_Stats.herbivores++;
                break;
            case CreatureType::Carnivore:
                m_Stats.carnivores++;
                break;
            case CreatureType::Omnivore:
                m_Stats.omnivores++;
                break;
            default:
                break;
        }
        
        totalAge += creature->getAge();
        totalHealth += creature->getHealth();
    }
    
    if (m_Stats.totalCreatures > 0) {
        m_Stats.averageAge = totalAge / m_Stats.totalCreatures;
        m_Stats.averageHealth = totalHealth / m_Stats.totalCreatures;
    } else {
        m_Stats.averageAge = 0.0f;
        m_Stats.averageHealth = 0.0f;
    }
}

void Simulation::reset() {
    m_Creatures.clear();
    m_Events.clear();
    m_SimulationTime = 0.0f;
    m_Stats = PopulationStats();
    
    logEvent(SimulationEvent::PopulationChange, EntityID(), "Simulation reset");
}

} // namespace EcoSystem
