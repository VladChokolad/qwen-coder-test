#include "SpeciesFactory.h"

namespace EcoSystem {

SpeciesParams createDefaultSpecies(CreatureType type) {
    SpeciesParams params;
    
    switch (type) {
        case CreatureType::Herbivore:
            params.name = "Rabbit";
            params.type = CreatureType::Herbivore;
            params.baseHealth = 50.0f;
            params.maxHealth = 50.0f;
            params.moveSpeed = 3.0f;
            params.perceptionRadius = 15.0f;
            params.hungerRate = 0.3f;
            params.reproductionThreshold = 40.0f;
            params.reproductionCooldown = 20.0f;
            params.offspringCount = 3;
            params.fearFactor = 0.8f;
            params.aggression = 0.1f;
            params.foodValue = 30.0f;
            break;
            
        case CreatureType::Carnivore:
            params.name = "Wolf";
            params.type = CreatureType::Carnivore;
            params.baseHealth = 100.0f;
            params.maxHealth = 100.0f;
            params.moveSpeed = 4.0f;
            params.perceptionRadius = 20.0f;
            params.attackDamage = 25.0f;
            params.attackRange = 2.0f;
            params.hungerRate = 0.5f;
            params.reproductionThreshold = 70.0f;
            params.reproductionCooldown = 40.0f;
            params.offspringCount = 2;
            params.aggression = 0.7f;
            params.fearFactor = 0.3f;
            params.foodValue = 60.0f;
            break;
            
        case CreatureType::Omnivore:
            params.name = "Bear";
            params.type = CreatureType::Omnivore;
            params.baseHealth = 150.0f;
            params.maxHealth = 150.0f;
            params.moveSpeed = 2.5f;
            params.perceptionRadius = 18.0f;
            params.attackDamage = 30.0f;
            params.attackRange = 2.5f;
            params.hungerRate = 0.6f;
            params.reproductionThreshold = 90.0f;
            params.reproductionCooldown = 50.0f;
            params.offspringCount = 2;
            params.aggression = 0.5f;
            params.foodValue = 80.0f;
            break;
            
        case CreatureType::Scavenger:
            params.name = "Vulture";
            params.type = CreatureType::Scavenger;
            params.baseHealth = 40.0f;
            params.maxHealth = 40.0f;
            params.moveSpeed = 5.0f;
            params.perceptionRadius = 30.0f;
            params.hungerRate = 0.4f;
            params.reproductionThreshold = 30.0f;
            params.reproductionCooldown = 15.0f;
            params.offspringCount = 4;
            params.aggression = 0.2f;
            params.fearFactor = 0.6f;
            params.foodValue = 20.0f;
            break;
            
        case CreatureType::Insect:
            params.name = "Beetle";
            params.type = CreatureType::Insect;
            params.baseHealth = 10.0f;
            params.maxHealth = 10.0f;
            params.moveSpeed = 1.5f;
            params.perceptionRadius = 5.0f;
            params.hungerRate = 0.2f;
            params.reproductionThreshold = 8.0f;
            params.reproductionCooldown = 5.0f;
            params.offspringCount = 10;
            params.aggression = 0.1f;
            params.fearFactor = 0.9f;
            params.foodValue = 5.0f;
            break;
            
        default:
            params.name = "Unknown";
            params.type = type;
            break;
    }
    
    return params;
}

} // namespace EcoSystem
