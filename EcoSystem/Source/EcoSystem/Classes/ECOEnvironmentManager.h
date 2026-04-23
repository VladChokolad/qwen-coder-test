// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ECOEnvironmentManager.generated.h"

USTRUCT(BlueprintType)
struct FECOSpeciesData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Species")
	FString SpeciesName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Species")
	TSubclassOf<class AECOCreature> CreatureClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Species")
	int32 InitialSpawnCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Species")
	ECOCreatureType Type;

	FECOSpeciesData() : InitialSpawnCount(10), Type(ECOCreatureType::Herbivore) {}
};

UCLASS()
class AECOENVIRONMENTMANAGER_API AECOEnvironmentManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AECOEnvironmentManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|Settings")
	float MapWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|Settings")
	float MapHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|Settings")
	TArray<FECOSpeciesData> SpeciesList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|Debug")
	bool bEnableDebugMode;

	UFUNCTION(BlueprintCallable, Category = "Environment|Management")
	void InitializeEcosystem();

	UFUNCTION(BlueprintCallable, Category = "Environment|Management")
	void SpawnSpecies(const FECOSpeciesData& SpeciesData);

	UFUNCTION(BlueprintCallable, Category = "Environment|Management")
	void GetPopulationStats(int32& TotalCount, int32& Herbivores, int32& Carnivores, int32& Omnivores);

	UFUNCTION(BlueprintCallable, Category = "Environment|Management")
	void CullOverpopulation();

private:
	TArray<AECOCreature*> AllCreatures;
	FTimerHandle PopulationCheckTimer;

	void UpdateCreatureList();
};
