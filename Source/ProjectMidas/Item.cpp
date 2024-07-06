// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AItem::ApplyAcceleration(FVector Acc)
{
	Body->AddForce(Acc, NAME_None, true);
}

// Called when the game starts or when spawned
void AItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	Body = FindComponentByClass<UPrimitiveComponent>();
	checkf(Body, TEXT("%s: no PrimitiveComponent on this Item"), *GetName());
}