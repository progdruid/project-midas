// Fill out your copyright notice in the Description page of Project Settings.


#include "AbyssPlayerControllerBase.h"

#include "Components/InputComponent.h"
#include "AbyssPawnBase.h"
#include "Item.h"

void AAbyssPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (MappingContext)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
		   GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		checkf(Subsystem, TEXT("No UEnhancedInputLocalPlayerSubsystem found."));
		Subsystem->AddMappingContext(MappingContext, 0);
	}
}

void AAbyssPlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	EIC = Cast<UEnhancedInputComponent>(InputComponent);
	checkf(EIC, TEXT("No UEnhancedInputComponent found on the player controller: %s."), *GetName());

	EIC->BindAction(InputDirectHorizontal, ETriggerEvent::Triggered, this, &AAbyssPlayerControllerBase::HandleDirectMotionInput);
	EIC->BindAction(InputSideHorizontal, ETriggerEvent::Triggered, this, &AAbyssPlayerControllerBase::HandleSideMotionInput);
	EIC->BindAction(InputVertical, ETriggerEvent::Triggered, this, &AAbyssPlayerControllerBase::HandleVerticalMotionInput);
	EIC->BindAction(InputLook, ETriggerEvent::Triggered, this, &AAbyssPlayerControllerBase::HandleLookInput);
}

void AAbyssPlayerControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AbyssPawn = Cast<AAbyssPawnBase>(InPawn);
	checkf(AbyssPawn, TEXT("The possessed pawn \"%s\" is not a subclass of AAbyssPawnBase."),
		*InPawn->GetName());
}

void AAbyssPlayerControllerBase::OnUnPossess()
{
	AbyssPawn = nullptr;
	
	Super::OnUnPossess();
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

void AAbyssPlayerControllerBase::HandleDirectMotionInput(const FInputActionValue& Value)
{
		
	if (AbyssPawn)
		AbyssPawn->MoveForward_Horizontal(Value.Get<float>());
}

void AAbyssPlayerControllerBase::HandleSideMotionInput(const FInputActionValue& Value)
{
	if (AbyssPawn)
		AbyssPawn->MoveRight_Horizontal(Value[0]);
}

void AAbyssPlayerControllerBase::HandleVerticalMotionInput(const FInputActionValue& Value)
{
	if (AbyssPawn)
		AbyssPawn->MoveUp_Vertical(Value[0]);
}

void AAbyssPlayerControllerBase::HandleLookInput(const FInputActionValue& Value)
{
	const float dt = GetWorld()->GetDeltaSeconds();
	AddYawInput(Value[0] * dt * LookRate);
	AddPitchInput(Value[1] * dt * LookRate);
}



