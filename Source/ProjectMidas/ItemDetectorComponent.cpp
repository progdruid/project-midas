// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemDetectorComponent.h"
#include "Item.h"

UItemDetectorComponent::UItemDetectorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	//SetVisibleFlag(false);
	UPrimitiveComponent::SetCollisionProfileName(FName("Trigger"));
}

void UItemDetectorComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UItemDetectorComponent::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UItemDetectorComponent::OnOverlapEnd);
}

void UItemDetectorComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AItem* Item = Cast<AItem>(OtherActor);
	if (!Item)
		return;

	OverlappingItems.Add(Item);

	OnItemEnterEvent.Broadcast(Item);
	
}

void UItemDetectorComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherCom, int32 OtherBodyIndex)
{
	AItem* Item = Cast<AItem>(OtherActor);
	if (!Item)
		return;

	OverlappingItems.Remove(Item);
	
	OnItemExitEvent.Broadcast(Item);
}

TArray<AItem*>& UItemDetectorComponent::GetItemsInsideCopy()
{
	return OverlappingItems;
}

const TArray<AItem*>& UItemDetectorComponent::GetItemsInside() const
{
	return OverlappingItems;
}
