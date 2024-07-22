// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"

// Sets default values
ACell::ACell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ACell::BeginPlay()
{
	Super::BeginPlay();

	AccountPrimitives();
	
	if (bSpawnedAsPrototype)
		ChangeToPrototype();
}

void ACell::AccountPrimitives()
{
	const bool bWasPrototype = bIsPrototype;
	ChangeToPrototype(true);
	
	HitVolume = Cast<UPrimitiveComponent>(GetDefaultSubobjectByName(HitVolumeName));
	if (HitVolume)
	{
		HitVolumeDefaultResponses = HitVolume->GetCollisionResponseToChannels();
		HitVolumeDefaultCollisionEnabled = HitVolume->GetCollisionEnabled();
	}
	
	//Other primitives
	TArray<UPrimitiveComponent*> Primitives;
	GetComponents<UPrimitiveComponent>(Primitives);
	if (HitVolume)
		Primitives.Remove(HitVolume);
	OtherPrimitivesAndCollision.Reset();
	for (const auto& Primitive : Primitives)
		OtherPrimitivesAndCollision.Add(Primitive, Primitive->GetCollisionEnabled());

	if (bWasPrototype)
		ChangeToPrototype();
}

void ACell::ChangeToPrototype(const bool bBack)
{
	if (bIsPrototype == !bBack || !HitVolume)
		return;
	
	bIsPrototype = !bBack;

	for (const auto& Pair : OtherPrimitivesAndCollision)
	{
		ECollisionEnabled::Type CET = ECollisionEnabled::NoCollision;
		if (bBack)
			CET = Pair.Value;

		Pair.Key->SetCollisionEnabled(CET);
	}
	
	if (bBack)
	{
		HitVolume->SetCollisionResponseToChannels(HitVolumeDefaultResponses);
		HitVolume->SetCollisionEnabled(HitVolumeDefaultCollisionEnabled);
	}
	else
		HitVolume->SetCollisionProfileName(FName("TriggerNoItems"));

	//change materials
}

bool ACell::CanGoBackFromPrototype() const
{
	if (!bIsPrototype)
		return false;

	TArray<AActor*> OverlappingActors;
	HitVolume->GetOverlappingActors(OverlappingActors);
	return  OverlappingActors.Num() == 0;
}

bool ACell::IsPrototype() const
{
	return bIsPrototype;
}


