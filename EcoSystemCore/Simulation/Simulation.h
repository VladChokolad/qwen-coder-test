#pragma once

#include "Creature.h"
#include "World.h"
#include <vector>
#include <memory>
#include <functional>
#include <queue>

namespace EcoSystem {

// События симуляции
enum class SimulationEvent {
    CreatureBorn,
    CreatureDied,
    CreatureAte,
    CreatureMated,
    PredatorAttack,
    PopulationChange
};

// Структура события
struct EventData {
    SimulationEvent type;
    EntityID creatureID;
    std::string description;
    float timestamp;
    
    EventData(SimulationEvent t, EntityID id, const std::string& desc, float time)
        : type(t), creatureID(id), description(desc), timestamp(time) {}
};

// Менеджер популяции для отслеживания статистики
struct PopulationStats {
    int totalCreatures = 0;
    int herbivores = 0;
    int carnivores = 0;
    int omnivores = 0;
    int births = 0;
    int deaths = 0;
    float averageAge = 0.0f;
    float averageHealth = 0.0f;
};

// Основной класс симуляции
class Simulation {
private:
    std::unique_ptr<World> m_World;
    std::vector<std::unique_ptr<Creature>> m_Creatures;
    
    // Параметры симуляции
    float m_TimeScale;           // Множитель скорости времени
    float m_SimulationTime;      // Общее время симуляции
    bool m_IsRunning;            // Запущена ли симуляция
    bool m_IsPaused;             // На паузе ли симуляция
    
    // Статистика
    PopulationStats m_Stats;
    std::vector<EventData> m_Events;
    size_t m_MaxEvents;          // Максимальное количество хранимых событий
    
    // Callbacks для внешних систем (например, визуализации)
    std::function<void(const EventData&)> m_EventCallback;
    std::function<void()> m_StateChangedCallback;
    
public:
    explicit Simulation(int worldWidth = 100, int worldHeight = 100, int worldDepth = 1);
    virtual ~Simulation() = default;
    
    // === Управление симуляцией ===
    
    // Запуск симуляции
    void start();
    
    // Остановка симуляции
    void stop();
    
    // Пауза/продолжение
    void pause();
    void resume();
    void togglePause();
    
    // Обновление симуляции (вызывается каждый кадр)
    void update(float deltaTime);
    
    // Установка масштаба времени (1.0 = нормальная скорость, 2.0 = ускорение x2)
    void setTimeScale(float scale);
    
    // === Управление миром ===
    
    // Генерация нового мира
    void generateWorld(unsigned int seed = 0);
    
    // Получение мира
    World& getWorld() { return *m_World; }
    const World& getWorld() const { return *m_World; }
    
    // === Управление существами ===
    
    // Добавление существа
    void addCreature(std::unique_ptr<Creature> creature);
    
    // Удаление существа по ID
    void removeCreature(EntityID id);
    
    // Удаление всех мертвых существ
    void cleanupDeadCreatures();
    
    // Получение существа по ID
    Creature* getCreature(EntityID id);
    const Creature* getCreature(EntityID id) const;
    
    // Получение всех существ
    std::vector<Creature*> getAllCreatures();
    std::vector<const Creature*> getAllCreatures() const;
    
    // Получение существ в радиусе
    std::vector<Creature*> getCreaturesInRadius(const Vector3& position, float radius);
    
    // Спавн случайного существа в случайной позиции
    void spawnRandomCreature();
    
    // Массовый спавн существ определенного вида
    void spawnCreatures(const SpeciesParams& species, int count);
    
    // Утилита для создания параметров вида по умолчанию
    friend SpeciesParams createDefaultSpecies(CreatureType type);
    
    // === Взаимодействия ===
    
    // Проверка и обработка взаимодействий между существами
    void processInteractions(float deltaTime);
    
    // Поиск ближайшей цели для существа
    Creature* findNearestTarget(Creature* creature, CreatureType preferredPrey);
    
    // Поиск пищи для травоядного
    Vector3 findFoodSource(const Vector3& position, float perceptionRadius);
    
    // Атака одного существа другим
    bool attack(Creature* attacker, Creature* target);
    
    // Поедание (труп или растение)
    void consume(Creature* creature, float foodValue);
    
    // === События и логирование ===
    
    // Регистрация события
    void logEvent(SimulationEvent type, EntityID creatureID, const std::string& description);
    
    // Получение последних событий
    const std::vector<EventData>& getRecentEvents(size_t count = 50) const;
    
    // Очистка лога событий
    void clearEvents();
    
    // Установка callback для событий
    void setEventCallback(std::function<void(const EventData&)> callback);
    
    // === Статистика ===
    
    // Обновление статистики популяции
    void updatePopulationStats();
    
    // Получение текущей статистики
    const PopulationStats& getPopulationStats() const { return m_Stats; }
    
    // Получение общего времени симуляции
    float getSimulationTime() const { return m_SimulationTime; }
    
    // Проверка состояния
    bool isRunning() const { return m_IsRunning; }
    bool isPaused() const { return m_IsPaused; }
    float getTimeScale() const { return m_TimeScale; }
    
    // === Утилиты ===
    
    // Сброс симуляции
    void reset();
    
    // Сохранение состояния (для будущего расширения)
    // void save(const std::string& filename);
    
    // Загрузка состояния (для будущего расширения)
    // void load(const std::string& filename);
};

} // namespace EcoSystem
