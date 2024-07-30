// Fill out your copyright notice in the Description page of Project Settings.


#include "AbyssPawn.h"

#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

AAbyssPawn::AAbyssPawn()
{
 	PrimaryActorTick.bCanEverTick = false;

	bCollideWhenPlacing = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionSphere0");
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->SetShouldUpdatePhysicsVolume(true);
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->bDynamicObstacle = true;
	checkf(CollisionComponent, TEXT("AbyssError: There is no body collision compoenent on this pawn. Bro, how?"))

	RootComponent = CollisionComponent;
	
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent0");
	MovementComponent->UpdatedComponent = CollisionComponent;
	checkf(MovementComponent, TEXT("AbyssError: There is no movement component on this pawn. Bro, how?"))

}

void AAbyssPawn::BeginPlay()
{
	Super::BeginPlay();
	
	World = GetWorld();
	checkf (World, TEXT("AbyssError: There is no world bro, how do you live like that?"));
}

void AAbyssPawn::UpdateNavigationRelevance()
{
	CollisionComponent->SetCanEverAffectNavigation(bCanAffectNavigationGeneration);
}

void AAbyssPawn::ToggleBoostedSpeed(bool Val)
{
	if (Val)
		MovementComponent->MaxSpeed = BoostedSpeed;
	else
		MovementComponent->MaxSpeed = NormalSpeed;
}

void AAbyssPawn::MoveForward_Horizontal(float Val)
{
	if (Val != 0 && Controller)
	{
		float S, C;
		FMath::SinCos(&S, &C, Controller->GetControlRotation().Yaw * (UE_PI / 180.f));
		const FVector Vec(C, S,0);
		MovementComponent->AddInputVector(Vec * Val);
	}
}

void AAbyssPawn::MoveRight_Horizontal(float Val)
{
	if (Val != 0 && Controller)
	{
		float S, C;
		FMath::SinCos(&S, &C, Controller->GetControlRotation().Yaw * (UE_PI / 180.f));
		const FVector Vec(-S, C,0);
		MovementComponent->AddInputVector(Vec * Val);
	}
}

void AAbyssPawn::MoveUp_Vertical(float Val)
{
	if (Val != 0.f)
		MovementComponent->AddInputVector(FVector::UpVector * Val);
}




