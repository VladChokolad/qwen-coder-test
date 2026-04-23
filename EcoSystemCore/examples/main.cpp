#include <iostream>
#include <chrono>
#include <thread>
#include "Simulation.h"
#include "SpeciesFactory.h"

using namespace EcoSystem;

int main() {
    std::cout << "=== EcoSystem Core Simulation Demo ===" << std::endl;
    
    // Создание симуляции с миром 50x50
    Simulation sim(50, 50, 1);
    
    // Генерация мира
    std::cout << "\nGenerating world..." << std::endl;
    sim.generateWorld(42);
    
    // Спавн начальной популяции
    std::cout << "Spawning initial population..." << std::endl;
    
    // Травоядные (кролики)
    SpeciesParams rabbitParams = createDefaultSpecies(CreatureType::Herbivore);
    sim.spawnCreatures(rabbitParams, 20);
    
    // Хищники (волки)
    SpeciesParams wolfParams = createDefaultSpecies(CreatureType::Carnivore);
    sim.spawnCreatures(wolfParams, 5);
    
    // Всеядные (медведи)
    SpeciesParams bearParams = createDefaultSpecies(CreatureType::Omnivore);
    sim.spawnCreatures(bearParams, 3);
    
    // Падальщики (грифы)
    SpeciesParams vultureParams = createDefaultSpecies(CreatureType::Scavenger);
    sim.spawnCreatures(vultureParams, 5);
    
    // Насекомые (жуки)
    SpeciesParams beetleParams = createDefaultSpecies(CreatureType::Insect);
    sim.spawnCreatures(beetleParams, 30);
    
    // Запуск симуляции
    sim.start();
    sim.setTimeScale(1.0f);
    
    std::cout << "\nSimulation started!" << std::endl;
    std::cout << "Press Ctrl+C to stop" << std::endl;
    
    // Главный цикл симуляции
    auto lastTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    const int maxFrames = 500;  // Ограничим количество кадров для демо
    
    try {
        while (frameCount < maxFrames && sim.isRunning()) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;
            
            // Обновление симуляции
            sim.update(deltaTime);
            
            // Вывод статистики каждые 60 кадров
            if (frameCount % 60 == 0) {
                const auto& stats = sim.getPopulationStats();
                std::cout << "\n[Time: " << sim.getSimulationTime() << "s] "
                          << "Population: " << stats.totalCreatures
                          << " (H: " << stats.herbivores 
                          << ", C: " << stats.carnivores
                          << ", O: " << stats.omnivores << ") "
                          << "| Births: " << stats.births
                          << ", Deaths: " << stats.deaths
                          << "| Avg Health: " << stats.averageHealth << std::endl;
                
                // Последние события
                const auto& events = sim.getRecentEvents(3);
                if (!events.empty()) {
                    std::cout << "Recent events:" << std::endl;
                    for (const auto& event : events) {
                        std::cout << "  - " << event.description << std::endl;
                    }
                }
            }
            
            frameCount++;
            
            // Небольшая задержка чтобы не перегружать консоль
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    // Финальная статистика
    std::cout << "\n=== Final Statistics ===" << std::endl;
    const auto& stats = sim.getPopulationStats();
    std::cout << "Total simulation time: " << sim.getSimulationTime() << "s" << std::endl;
    std::cout << "Final population: " << stats.totalCreatures << std::endl;
    std::cout << "  - Herbivores: " << stats.herbivores << std::endl;
    std::cout << "  - Carnivores: " << stats.carnivores << std::endl;
    std::cout << "  - Omnivores: " << stats.omnivores << std::endl;
    std::cout << "Total births: " << stats.births << std::endl;
    std::cout << "Total deaths: " << stats.deaths << std::endl;
    std::cout << "Average age: " << stats.averageAge << "s" << std::endl;
    std::cout << "Average health: " << stats.averageHealth << std::endl;
    
    // Остановка симуляции
    sim.stop();
    
    std::cout << "\nDemo completed successfully!" << std::endl;
    return 0;
}
