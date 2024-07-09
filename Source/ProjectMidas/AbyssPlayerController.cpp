// Fill out your copyright notice in the Description page of Project Settings.


#include "AbyssPlayerController.h"

#include "Components/InputComponent.h"
#include "AbyssPawn.h"
#include "GeometryTypes.h"
#include "Item.h"

void AAbyssPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (MappingContext)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
		   GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		ensureMsgf(Subsystem, TEXT("No UEnhancedInputLocalPlayerSubsystem found."));
		if (Subsystem)
			Subsystem->AddMappingContext(MappingContext, 0);
	}
}

void AAbyssPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	EIC = Cast<UEnhancedInputComponent>(InputComponent);
	ensureMsgf(EIC, TEXT("AbyssError: No UEnhancedInputComponent found on the player controller: %s."), *GetName());
	if (!EIC)
		return;
	
	ensureMsgf(InputDirectHorizontal, TEXT("AbyssError: InputDirectHorizontal empty on PC: %s."), *GetName());
	ensureMsgf(InputSideHorizontal, TEXT("AbyssError: InputSideHorizontal empty on PC: %s."), *GetName());
	ensureMsgf(InputVertical, TEXT("AbyssError: InputVertical empty on PC: %s."), *GetName());
	ensureMsgf(InputLook, TEXT("AbyssError: InputLook empty on PC: %s."), *GetName());
	ensureMsgf(InputChangeLookState, TEXT("AbyssError: InputChangeLookState empty on PC: %s."), *GetName());
	
	if (InputDirectHorizontal)
		EIC->BindAction(InputDirectHorizontal, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleDirectMotionInput);
	if (InputSideHorizontal)
		EIC->BindAction(InputSideHorizontal, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleSideMotionInput);
	if (InputVertical)
		EIC->BindAction(InputVertical, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleVerticalMotionInput);
	if (InputLook)
		EIC->BindAction(InputLook, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleLookInput);
	if (InputChangeLookState)
		EIC->BindAction(InputChangeLookState, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleLookChangeInput);
}

void AAbyssPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AbyssPawn = Cast<AAbyssPawn>(InPawn);
	ensureMsgf(AbyssPawn, TEXT("AbyssError: The possessed pawn %s is not a subclass of AAbyssPawn."), *InPawn->GetName());
}

void AAbyssPlayerController::OnUnPossess()
{
	AbyssPawn = nullptr;
	
	Super::OnUnPossess();
}

void AAbyssPlayerController::Tick(const float DeltaSeconds)
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



void AAbyssPlayerController::ChangeItemDrag(const bool bInDrag)
{
	DraggedItem = nullptr;
	
	if (bInDrag)
	{
		FHitResult Hit;
		TraceForward(Hit, ECollisionChannel::ECC_PhysicsBody);
		DraggedItem = Cast<AItem>(Hit.GetActor());
	}
}

bool AAbyssPlayerController::TraceForward(FHitResult& Hit, ECollisionChannel Channel) const
{
	FVector StartLocation;
	FRotator ViewRotation;
	GetPlayerViewPoint(StartLocation, ViewRotation);
	const FVector EndLocation = StartLocation + (ViewRotation.Vector() * TraceDistance);

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetPawn());

	const bool found = GetWorld()->LineTraceSingleByChannel(
		Hit,
		StartLocation,
		EndLocation,
		Channel,
		TraceParams
	);
	
	return found;
}

void AAbyssPlayerController::HandleDirectMotionInput(const FInputActionValue& Value)
{
		
	if (AbyssPawn)
		AbyssPawn->MoveForward_Horizontal(Value.Get<float>());
}

void AAbyssPlayerController::HandleSideMotionInput(const FInputActionValue& Value)
{
	if (AbyssPawn)
		AbyssPawn->MoveRight_Horizontal(Value[0]);
}

void AAbyssPlayerController::HandleVerticalMotionInput(const FInputActionValue& Value)
{
	if (AbyssPawn)
		AbyssPawn->MoveUp_Vertical(Value[0]);
}

void AAbyssPlayerController::HandleLookChangeInput(const FInputActionValue& Value)
{
	static FVector2f SavedMousePos;
	
	bInLook = Value.GetMagnitude() > KINDA_SMALL_NUMBER;
	SetShowMouseCursor(!bInLook);
	if (bInLook)
		GetMousePosition(SavedMousePos.X, SavedMousePos.Y);
	else
		SetMouseLocation(SavedMousePos.X, SavedMousePos.Y);
}

void AAbyssPlayerController::HandleLookInput(const FInputActionValue& Value)
{
	if(!bInLook)
		return;
	
	const float dt = GetWorld()->GetDeltaSeconds();
	AddYawInput(Value[0] * dt * LookRate);
	AddPitchInput(Value[1] * dt * LookRate);
}



