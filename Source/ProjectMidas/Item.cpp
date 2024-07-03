// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Body = Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f,
		FColor::Red, FString("Body: ") + FString::FromInt(!!Body));
}

void AItem::ApplyAcceleration(FVector Acc)
{
	if (Body)
		Body->AddForce(Acc, NAME_None, true);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}
