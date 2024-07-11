// Fill out your copyright notice in the Description page of Project Settings.


#include "AbyssPlayerController.h"
#include "Widgets/SViewport.h"
#include "Framework/Application/SlateApplication.h"
#include "Input/Reply.h"
#include "Components/InputComponent.h"
#include "AbyssPawn.h"
#include "Item.h"

void AAbyssPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* World = GetWorld();
	ensureMsgf(World, TEXT("AbyssError: No world found."));
	if (World)
	{
		GameViewportClient = World->GetGameViewport();
		ensureMsgf(GameViewportClient, TEXT("AbyssError: No GameViewportClient found."));
	}
	
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	ensureMsgf(LocalPlayer, TEXT("AbyssError: No LocalPlayer found."));

	UEnhancedInputLocalPlayerSubsystem* EISubsystem = nullptr;
	
	if (LocalPlayer)
	{
		SlateOperations = &LocalPlayer->GetSlateOperations();
		
		EISubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		ensureMsgf(EISubsystem, TEXT("No UEnhancedInputLocalPlayerSubsystem found."));
	}

	if (EISubsystem && MappingContext)
		EISubsystem->AddMappingContext(MappingContext, 0);
	
	ToggleImmersiveMode(false);
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
	ensureMsgf(InputLookMotion, TEXT("AbyssError: InputLook empty on PC: %s."), *GetName());
	ensureMsgf(InputTurnChange, TEXT("AbyssError: InputChangeLookState empty on PC: %s."), *GetName());
	ensureMsgf(InputInteract, TEXT("AbyssError: InputLocationInteract empty on PC: %s."), *GetName());
	
	if (InputDirectHorizontal)
		EIC->BindAction(InputDirectHorizontal, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleDirectMotionInput);
	if (InputSideHorizontal)
		EIC->BindAction(InputSideHorizontal, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleSideMotionInput);
	if (InputVertical)
		EIC->BindAction(InputVertical, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleVerticalMotionInput);
	if (InputTurnChange)
		EIC->BindAction(InputTurnChange, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleTurnChangeInput);
	if (InputInteract)
		EIC->BindAction(InputInteract, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleInteractInput);
	if (InputLookMotion)
		EIC->BindAction(InputLookMotion, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleLookMotionInput);
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
		FVector Acc = DraggedItemTarget - DraggedItem->GetActorLocation();
		Acc.Normalize();
		Acc *= DragAccelerationCoefficient;
		DraggedItem->ApplyAcceleration(Acc);
	}
}


bool AAbyssPlayerController::TraceAtScreenPos(FHitResult& Hit, ECollisionChannel Channel,
	const FVector2f& ScreenPos) const
{
	FVector Start;
	FVector Direction;
	DeprojectScreenPositionToWorld(ScreenPos.X, ScreenPos.Y, Start, Direction);
	const FVector End = Start + (Direction * TraceDistance);

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetPawn());

	return GetWorld()->LineTraceSingleByChannel(Hit, Start, End, Channel, TraceParams);
}


void AAbyssPlayerController::ToggleImmersiveMode(bool Value)
{
	if (!GameViewportClient || !SlateOperations)
		return;
	
	TSharedPtr<SViewport> ViewportWidget = GameViewportClient->GetGameViewportWidget();
	if (ViewportWidget.IsValid())
	{
		TSharedRef<SViewport> ViewportWidgetRef = ViewportWidget.ToSharedRef();
		SlateOperations->UseHighPrecisionMouseMovement(ViewportWidgetRef);
		
		//SlateOperations->SetUserFocus(ViewportWidgetRef);
		//SlateOperations->LockMouseToWidget(ViewportWidgetRef);
		//GameViewportClient->SetMouseLockMode(EMouseLockMode::LockOnCapture);
		//GameViewportClient->SetIgnoreInput(false);
		//GameViewportClient->SetMouseCaptureMode(bConsumeCaptureMouseDown ? EMouseCaptureMode::CapturePermanently : EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
	}
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

void AAbyssPlayerController::HandleTurnChangeInput(const FInputActionValue& Value)
{
	bInLook = Value.GetMagnitude() > KINDA_SMALL_NUMBER;

	SetShowMouseCursor(!bInLook);
	ToggleImmersiveMode(bInLook);
	
	if (bInLook)
		GetMousePosition(SavedMousePos.X, SavedMousePos.Y);
	else
		SetMouseLocation(SavedMousePos.X, SavedMousePos.Y);
}

void AAbyssPlayerController::HandleInteractInput(const FInputActionValue& Value)
{
	DraggedItem = nullptr;
	
	if (!bInLook && Value.GetMagnitude() > KINDA_SMALL_NUMBER)
	{
		FVector2f ScreenPos;
		GetMousePosition(ScreenPos.X, ScreenPos.Y);
		FHitResult Hit;
		TraceAtScreenPos(Hit, ECollisionChannel::ECC_PhysicsBody, ScreenPos);
		DraggedItem = Cast<AItem>(Hit.GetActor());
	}
}

void AAbyssPlayerController::HandleLookMotionInput(const FInputActionValue& Value)
{
	FVector2f CursorPos;	
	if(bInLook)
	{
		CursorPos = SavedMousePos;
		const float dt = GetWorld()->GetDeltaSeconds();
		AddYawInput(Value[0] * dt * LookRate);
		AddPitchInput(Value[1] * dt * LookRate);	
	}
	else
		GetMousePosition(CursorPos.X, CursorPos.Y);

	//set target location
	if (DraggedItem)
	{
		FHitResult Hit;
		if (TraceAtScreenPos(Hit, ECC_WorldStatic, CursorPos))
			DraggedItemTarget = Hit.Location;
		
	}
}

