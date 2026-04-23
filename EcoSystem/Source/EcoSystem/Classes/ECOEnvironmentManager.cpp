// Copyright Epic Games, Inc. All Rights Reserved.

#include "ECOEnvironmentManager.h"
#include "ECOCreature.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

AECOEnvironmentManager::AECOEnvironmentManager()
{
	PrimaryActorTick.bCanEverTick = true;

	MapWidth = 10000.0f;
	MapHeight = 10000.0f;
	bEnableDebugMode = true;
}

void AECOEnvironmentManager::BeginPlay()
{
	Super::BeginPlay();

	InitializeEcosystem();

	GetWorldTimerManager().SetTimer(
		PopulationCheckTimer, 
		this, 
		&AECOEnvironmentManager::UpdateCreatureList, 
		5.0f, 
		true
	);
}

void AECOEnvironmentManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AECOEnvironmentManager::InitializeEcosystem()
{
	for (const FECOSpeciesData& Species : SpeciesList)
	{
		SpawnSpecies(Species);
	}
}

void AECOEnvironmentManager::SpawnSpecies(const FECOSpeciesData& SpeciesData)
{
	if (!SpeciesData.CreatureClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Creature class for species %s is not set!"), *SpeciesData.SpeciesName);
		return;
	}

	for (int32 i = 0; i < SpeciesData.InitialSpawnCount; ++i)
	{
		float X = FMath::RandRange(-MapWidth / 2.0f, MapWidth / 2.0f);
		float Y = FMath::RandRange(-MapHeight / 2.0f, MapHeight / 2.0f);
		FVector SpawnLocation(X, Y, 100.0f);
		FRotator SpawnRotation(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);

		AECOCreature* Creature = GetWorld()->SpawnActor<AECOCreature>(
			SpeciesData.CreatureClass, 
			SpawnLocation, 
			SpawnRotation
		);

		if (Creature)
		{
			Creature->CreatureType = SpeciesData.Type;
			AllCreatures.Add(Creature);
			
			if (bEnableDebugMode)
			{
				UE_LOG(LogTemp, Log, TEXT("Spawned %s at %s"), *SpeciesData.SpeciesName, *SpawnLocation.ToString());
			}
		}
	}
}

void AECOEnvironmentManager::GetPopulationStats(int32& TotalCount, int32& Herbivores, int32& Carnivores, int32& Omnivores)
{
	UpdateCreatureList();

	TotalCount = AllCreatures.Num();
	Herbivores = 0;
	Carnivores = 0;
	Omnivores = 0;

	for (AECOCreature* Creature : AllCreatures)
	{
		if (Creature && !Creature->IsPendingKill())
		{
			switch (Creature->CreatureType)
			{
			case ECOCreatureType::Herbivore:
				Herbivores++;
				break;
			case ECOCreatureType::Carnivore:
				Carnivores++;
				break;
			case ECOCreatureType::Omnivore:
				Omnivores++;
				break;
			}
		}
	}

	if (bEnableDebugMode)
	{
		UE_LOG(LogTemp, Display, 
			TEXT("Population Stats - Total: %d, Herbivores: %d, Carnivores: %d, Omnivores: %d"),
			TotalCount, Herbivores, Carnivores, Omnivores);
	}
}

void AECOEnvironmentManager::CullOverpopulation()
{
	const int32 MaxPopulation = 500;
	
	UpdateCreatureList();

	if (AllCreatures.Num() > MaxPopulation)
	{
		int32 ToRemove = AllCreatures.Num() - MaxPopulation;
		
		for (int32 i = 0; i < ToRemove && AllCreatures.Num() > 0; ++i)
		{
			int32 RandomIndex = FMath::RandRange(0, AllCreatures.Num() - 1);
			AECOCreature* Creature = AllCreatures[RandomIndex];
			
			if (Creature && !Creature->IsPendingKill())
			{
				Creature->Destroy();
				AllCreatures.RemoveAtSwap(RandomIndex);
			}
		}

		if (bEnableDebugMode)
		{
			UE_LOG(LogTemp, Warning, TEXT("Culled %d creatures due to overpopulation."), ToRemove);
		}
	}
}

void AECOEnvironmentManager::UpdateCreatureList()
{
	for (int32 i = AllCreatures.Num() - 1; i >= 0; --i)
	{
		if (!AllCreatures[i] || AllCreatures[i]->IsPendingKill())
		{
			AllCreatures.RemoveAtSwap(i);
		}
	}

	CullOverpopulation();
}
