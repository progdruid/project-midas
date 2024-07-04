// Fill out your copyright notice in the Description page of Project Settings.


#include "AbyssPlayerControllerBase.h"

AAbyssPlayerControllerBase::AAbyssPlayerControllerBase()
{
	
}

void AAbyssPlayerControllerBase::ChangeItemDrag(const bool bInDrag)
{
	DraggedItem = nullptr;
	
	if (bInDrag)
	{
		FHitResult Hit;
		TraceForward(Hit, ECollisionChannel::ECC_PhysicsBody);
		DraggedItem = Cast<AItem>(Hit.GetActor());
	}
}

bool AAbyssPlayerControllerBase::TraceForward(FHitResult& Hit, ECollisionChannel Channel) const
{
	FVector StartLocation;
	FRotator ViewRotation;
	GetPlayerViewPoint(StartLocation, ViewRotation);
	const FVector EndLocation = StartLocation + (ViewRotation.Vector() * TraceDistance);

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetPawn());

	bool found = GetWorld()->LineTraceSingleByChannel(
		Hit,
		StartLocation,
		EndLocation,
		Channel,
		TraceParams
	);

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f,
		FColor::Red, FString("Hit: ") + FString::FromInt(found));
	
	return found;
}

void AAbyssPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAbyssPlayerControllerBase::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (DraggedItem)
	{
		FHitResult Hit;
		if (TraceForward(Hit, ECC_WorldStatic))
		{
			FVector Acc = Hit.Location - DraggedItem->GetActorLocation();
			Acc.Normalize();
			Acc *= DragAccelerationCoefficient;
			DraggedItem->ApplyAcceleration(Acc);
		}
	}
}

