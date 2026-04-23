// Copyright Epic Games, Inc. All Rights Reserved.

#include "ECOCreature.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AECOCreature::AECOCreature()
{
	PrimaryActorTick.bCanEverTick = true;

	CreatureType = ECOCreatureType::Herbivore;
	
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	
	HungerLevel = 0.0f;
	MaxHunger = 100.0f;
	
	MoveSpeed = 300.0f;
	PerceptionRadius = 1500.0f;
	
	ReproductionThreshold = 80.0f;
	CurrentEnergy = 50.0f;
}

void AECOCreature::BeginPlay()
{
	Super::BeginPlay();
}

void AECOCreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateVitals(DeltaTime);

	if (CurrentHealth <= 0.0f)
	{
		Die();
		return;
	}

	if (HungerLevel >= MaxHunger)
	{
		SearchForFood();
	}
	else if (CurrentEnergy >= ReproductionThreshold)
	{
		Reproduce();
	}
	else
	{
		Wander();
	}
}

void AECOCreature::UpdateVitals(float DeltaTime)
{
	HungerLevel += DeltaTime * 2.0f;
	
	if (HungerLevel >= MaxHunger)
	{
		CurrentHealth -= DeltaTime * 5.0f;
	}
	
	CurrentEnergy = FMath::Clamp(CurrentEnergy, 0.0f, 100.0f);
}

void AECOCreature::SearchForFood()
{
	AActor* NearestTarget = nullptr;
	float NearestDistance = MAX_FLT;

	if (CreatureType == ECOCreatureType::Herbivore || CreatureType == ECOCreatureType::Omnivore)
	{
		FindNearestTarget(NearestTarget, NearestDistance, AActor::StaticClass());
	}
	
	if (CreatureType == ECOCreatureType::Carnivore || CreatureType == ECOCreatureType::Omnivore)
	{
		AActor* Prey = nullptr;
		float PreyDistance = MAX_FLT;
		FindNearestTarget(Prey, PreyDistance, AECOCreature::StaticClass());
		
		if (Prey && PreyDistance < NearestDistance)
		{
			NearestTarget = Prey;
			NearestDistance = PreyDistance;
		}
	}

	if (NearestTarget)
	{
		FVector Location = GetActorLocation();
		FVector TargetLocation = NearestTarget->GetActorLocation();
		FVector Direction = (TargetLocation - Location).GetSafeNormal();

		AddActorWorldOffset(Direction * MoveSpeed * GetWorld()->GetDeltaSeconds(), true);
		SetActorRotation(Direction.Rotation());
	}
}

void AECOCreature::Eat(float NutritionValue)
{
	HungerLevel = FMath::Clamp(HungerLevel - NutritionValue, 0.0f, MaxHunger);
	CurrentEnergy = FMath::Clamp(CurrentEnergy + (NutritionValue * 0.5f), 0.0f, 100.0f);
}

void AECOCreature::Reproduce()
{
	if (CurrentEnergy >= ReproductionThreshold)
	{
		CurrentEnergy -= ReproductionThreshold;
		HungerLevel += 20.0f;
		
		FVector SpawnLocation = GetActorLocation() + FVector(FMath::RandRange(-100.0f, 100.0f), FMath::RandRange(-100.0f, 100.0f), 0.0f);
		FRotator SpawnRotation = GetActorRotation();
		
		UWorld* World = GetWorld();
		if (World)
		{
			World->SpawnActor<AECOCreature>(AECOCreature::StaticClass(), SpawnLocation, SpawnRotation);
		}
	}
}

void AECOCreature::Wander()
{
	FVector RandomDirection = FVector(
		FMath::RandRange(-1.0f, 1.0f),
		FMath::RandRange(-1.0f, 1.0f),
		0.0f
	).GetSafeNormal();

	AddActorWorldOffset(RandomDirection * MoveSpeed * 0.5f * GetWorld()->GetDeltaSeconds(), true);
	SetActorRotation(RandomDirection.Rotation());
}

void AECOCreature::Die()
{
	Destroy();
}

void AECOCreature::FindNearestTarget(AActor*& OutTarget, float& OutDistance, TSubclassOf<AActor> TargetClass)
{
	OutTarget = nullptr;
	OutDistance = MAX_FLT;

	FVector MyLocation = GetActorLocation();
	TArray<FHitResult> HitResults;

	FCollisionShape SphereShape = FCollisionShape::MakeSphere(PerceptionRadius);
	
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		MyLocation,
		MyLocation,
		FQuat::Identity,
		ECC_Pawn,
		SphereShape
	);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			if (Hit.GetActor() && Hit.GetActor() != this && Hit.GetActor()->IsA(TargetClass))
			{
				float Dist = FVector::Dist(MyLocation, Hit.GetActor()->GetActorLocation());
				if (Dist < OutDistance)
				{
					OutDistance = Dist;
					OutTarget = Hit.GetActor();
				}
			}
		}
	}
}
