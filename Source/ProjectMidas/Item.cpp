// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	Body = FindComponentByClass<UPrimitiveComponent>();
	ensureMsgf(Body, TEXT("%s: no PrimitiveComponent on this Item"), *GetName());
}

void AItem::ApplyAcceleration(FVector Acc)
{
	Body->AddForce(Acc, NAME_None, true);
}

void AItem::ApplySweep(FVector Sweep, float SpeedLimit)
{
	const float SpeedTowardsSweep = FVector::DotProduct(Sweep.GetSafeNormal(), Body->GetPhysicsLinearVelocity());
	if (SpeedTowardsSweep < 0)
		ApplyAcceleration(Sweep * 2);
	else if (SpeedTowardsSweep < SpeedLimit || SpeedLimit < KINDA_SMALL_NUMBER)
		ApplyAcceleration(Sweep);
}

void AItem::ToggleGravity(bool Value)
{
	Body->SetEnableGravity(Value);
}

void AItem::SetVelocity(FVector Vel)
{
	Body->SetPhysicsLinearVelocity(Vel);
}

void AItem::AddVelocity(FVector Vel)
{
	Body->SetPhysicsLinearVelocity(Vel, true);
}

FVector AItem::GetVelocity() const
{
	if (Body)
		return Body->GetPhysicsLinearVelocity();

	return FVector::ZeroVector;
}
