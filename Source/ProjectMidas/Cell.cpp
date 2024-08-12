// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ACell::ACell()
{
	PrimaryActorTick.bCanEverTick = bTickWhileCell || bTickWhilePrototype;
}

void ACell::BeginPlay()
{
	Super::BeginPlay();
	
	AccountPrimitives();

	if (bSpawnedAsPrototype)
	{
		ChangeToPrototype();
		return;
	}

	SetActorTickEnabled(bTickWhileCell);
	CellStart();
}

void ACell::AccountPrimitives()
{
	const bool bWasPrototype = bIsPrototype;
	ChangeBackToNormal();
	
	HitVolume = Cast<UPrimitiveComponent>(GetDefaultSubobjectByName(HitVolumeName));
	if (HitVolume)
	{
		HitVolumeDefaultCollisionEnabled = HitVolume->GetCollisionEnabled();
		HitVolumeDefaultObjectType = HitVolume->GetCollisionObjectType();
		HitVolumeDefaultResponses = HitVolume->GetCollisionResponseToChannels();
	}
	
	//Other primitives
	TArray<UPrimitiveComponent*> Primitives;
	GetComponents<UPrimitiveComponent>(Primitives);
	
	if (HitVolume)
		Primitives.Remove(HitVolume);
	for (const FName& IntactVolumeName : IntactVolumeNames)
	{
		UPrimitiveComponent* IntactVolume = Cast<UPrimitiveComponent>(GetDefaultSubobjectByName(IntactVolumeName));
		if (IntactVolume)
			Primitives.Remove(IntactVolume);
	}

	OtherPrimitivesAndCollision.Reset();
	for (const auto& Primitive : Primitives)
		OtherPrimitivesAndCollision.Add(Primitive, Primitive->GetCollisionEnabled());

	if (bWasPrototype)
		ChangeToPrototype();
}

void ACell::ChangeToPrototype()
{
	if (bIsPrototype)
		return;
	

	//events
	CellEnd();

	
	//actor state
	bIsPrototype = true;
	SetActorTickEnabled(bTickWhilePrototype);

	
	//change collisions
	for (const auto& Pair : OtherPrimitivesAndCollision)
		Pair.Key->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (HitVolume)
		HitVolume->SetCollisionProfileName(FName("TriggerNoItems"));

	
	//change materials
	//--------------
	//
	//--------------
}

void ACell::ChangeBackToNormal()
{
	if (!bIsPrototype)
		return;
	
	//actor state
	bIsPrototype = false;
	SetActorTickEnabled(bTickWhileCell);

	
	//change collisions
	for (const auto& Pair : OtherPrimitivesAndCollision)
		Pair.Key->SetCollisionEnabled(Pair.Value);

	if (HitVolume)
	{
		HitVolume->SetCollisionEnabled(HitVolumeDefaultCollisionEnabled);
		HitVolume->SetCollisionObjectType(HitVolumeDefaultObjectType);
		HitVolume->SetCollisionResponseToChannels(HitVolumeDefaultResponses);
	}

	
	//events
	CellStart();
}

bool ACell::CanGoBackFromPrototype() const
{
	if (!bIsPrototype)
		return false;
	
	if (!HitVolume)
		return true;
	
	TArray<AActor*> OverlappingActors;
	HitVolume->GetOverlappingActors(OverlappingActors);
	return  OverlappingActors.Num() == 0;
}

bool ACell::IsPrototype() const
{
	return bIsPrototype;
}

bool ACell::AreRequirementsMet_Implementation()
{
	return true;
}


