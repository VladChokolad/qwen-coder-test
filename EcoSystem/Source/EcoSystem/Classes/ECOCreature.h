// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ECOCreature.generated.h"

UENUM(BlueprintType)
enum class ECOCreatureType : uint8
{
	Herbivore	UMETA(DisplayName = "Herbivore"),
	Carnivore	UMETA(DisplayName = "Carnivore"),
	Omnivore	UMETA(DisplayName = "Omnivore")
};

UCLASS()
class AECOCREATURE_API AECOCreature : public AActor
{
	GENERATED_BODY()
	
public:	
	AECOCreature();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creature")
	ECOCreatureType CreatureType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creature|Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creature|Stats")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creature|Stats")
	float HungerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creature|Stats")
	float MaxHunger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creature|Stats")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creature|Stats")
	float PerceptionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creature|Reproduction")
	float ReproductionThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creature|Reproduction")
	float CurrentEnergy;

	UFUNCTION(BlueprintCallable, Category = "Creature|Actions")
	void SearchForFood();

	UFUNCTION(BlueprintCallable, Category = "Creature|Actions")
	void Eat(float NutritionValue);

	UFUNCTION(BlueprintCallable, Category = "Creature|Actions")
	void Reproduce();

	UFUNCTION(BlueprintCallable, Category = "Creature|Actions")
	void Wander();

	UFUNCTION(BlueprintCallable, Category = "Creature|Internal")
	void UpdateVitals(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Creature|Internal")
	void Die();

private:
	void FindNearestTarget(AActor*& OutTarget, float& OutDistance, TSubclassOf<AActor> TargetClass);
};
