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
	else if (OnCellStart.IsBound())
		OnCellStart.Broadcast();
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
	if (OnCellEnd.IsBound())
		OnCellEnd.Broadcast();

	
	//actor state
	bIsPrototype = true;
	SetActorTickEnabled(false);

	
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
	SetActorTickEnabled(true);

	
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
	if (OnCellStart.IsBound())
		OnCellStart.Broadcast();
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


