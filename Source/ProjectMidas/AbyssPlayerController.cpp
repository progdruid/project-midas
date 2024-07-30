// Fill out your copyright notice in the Description page of Project Settings.


#include "AbyssPlayerController.h"
#include "Widgets/SViewport.h"
#include "Framework/Application/SlateApplication.h"
#include "Input/Reply.h"
#include "Components/InputComponent.h"
#include "AbyssPawn.h"
#include "Cell.h"
#include "DrawDebugHelpers.h"
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
	
	EnableImmersiveMode();
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
	ensureMsgf(InputSpeedBoost, TEXT("AbyssError: InputSpeedBoost empty on PC: %s."), *GetName());
	ensureMsgf(InputLookMotion, TEXT("AbyssError: InputLook empty on PC: %s."), *GetName());
	ensureMsgf(InputTurnChange, TEXT("AbyssError: InputChangeLookState empty on PC: %s."), *GetName());
	ensureMsgf(InputInteract, TEXT("AbyssError: InputLocationInteract empty on PC: %s."), *GetName());
	ensureMsgf(InputConstructionToggle, TEXT("AbyssError: InputConstructionToggle empty on PC: %s."), *GetName());
	ensureMsgf(InputSelectedCellChange, TEXT("AbyssError: InputSelectedCellChange empty on PC: %s."), *GetName());
	ensureMsgf(InputCellRotation, TEXT("AbyssError: InputCellRotation empty on PC: %s."), *GetName());
	
	if (InputDirectHorizontal)
		EIC->BindAction(InputDirectHorizontal, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleDirectMotionInput);
	if (InputSideHorizontal)
		EIC->BindAction(InputSideHorizontal, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleSideMotionInput);
	if (InputVertical)
		EIC->BindAction(InputVertical, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleVerticalMotionInput);
	if (InputSpeedBoost)
		EIC->BindAction(InputSpeedBoost, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleSpeedBoostInput);
	if (InputTurnChange)
		EIC->BindAction(InputTurnChange, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleTurnChangeInput);
	if (InputInteract)
		EIC->BindAction(InputInteract, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleInteractInput);
	if (InputLookMotion)
		EIC->BindAction(InputLookMotion, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleCursorPosChangeInput);
	if (InputConstructionToggle)
		EIC->BindAction(InputConstructionToggle, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleConstructionModeToggleInput);
	if (InputSelectedCellChange)
		EIC->BindAction(InputSelectedCellChange, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleSelectedCellChangeInput);
	if (InputCellRotation)
		EIC->BindAction(InputCellRotation, ETriggerEvent::Triggered, this, &AAbyssPlayerController::HandleCellRotationInput);
		
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

	const FVector2f CursorPos = GetCurrentInteractionCursorPosition();
	FHitResult Hit;
	if(!TraceAtScreenPos(Hit, ECollisionChannel::ECC_WorldStatic, CursorPos))
		return;
	
	if (CurrentInteractionMode == EInteractionMode::ItemDrag && DraggedItem)
	{
		FVector Acc = Hit.Location - DraggedItem->GetActorLocation();
		Acc.Normalize();
		Acc *= DragAccelerationCoefficient;
		DraggedItem->ApplyAcceleration(Acc);
	}
	else if (CurrentInteractionMode == EInteractionMode::Construction)
		PlaceCellPrototypeAtHit(PrototypeCell, Hit);
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

FVector2f AAbyssPlayerController::GetCurrentInteractionCursorPosition() const
{
	if (bInLook)
		return SavedInteractionCursorPos;

	FVector2f ScreenPos;
	GetMousePosition(ScreenPos.X, ScreenPos.Y);
	return ScreenPos;
}


void AAbyssPlayerController::EnableImmersiveMode()
{
	if (!GameViewportClient || !SlateOperations)
		return;

	const TSharedPtr<SViewport> ViewportWidget = GameViewportClient->GetGameViewportWidget();
	if (ViewportWidget.IsValid())
	{
		const TSharedRef<SViewport> ViewportWidgetRef = ViewportWidget.ToSharedRef();
		SlateOperations->UseHighPrecisionMouseMovement(ViewportWidgetRef);
		
		//SlateOperations->SetUserFocus(ViewportWidgetRef);
		//SlateOperations->LockMouseToWidget(ViewportWidgetRef);
		//GameViewportClient->SetMouseLockMode(EMouseLockMode::LockOnCapture);
		//GameViewportClient->SetIgnoreInput(false);
		//GameViewportClient->SetMouseCaptureMode(bConsumeCaptureMouseDown ? EMouseCaptureMode::CapturePermanently : EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
	}
}




//construction implementation

void AAbyssPlayerController::ResetCellPrototype()
{
	if ((!PrototypeCell || !PrototypeCell->Destroy()) && PrototypeCell)
		return;

	PrototypeCell = nullptr;	

	if (CurrentInteractionMode != EInteractionMode::Construction)
		return;
	
	const TSubclassOf<ACell>& SelectedCell = ConstructableCells[SelectedConstructionCellIndex];
	if (!SelectedCell.GetDefaultObject())
		return;
	
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.CustomPreSpawnInitalization = [](AActor* Actor)->void
	{
		ACell* Cell = CastChecked<ACell>(Actor);
		Cell->bSpawnedAsPrototype = true;
	};
	
	PrototypeCell = CastChecked<ACell>(GetWorld()->SpawnActor(SelectedCell, nullptr, nullptr, Params));
}

void AAbyssPlayerController::PlaceCellPrototypeAtHit(ACell* Cell, const FHitResult& Hit) const
{
	if (!Cell)
		return;

	//const FBox& Box = Cell->GetComponentsBoundingBox();
	const FVector Location = Hit.Location;// + Hit.Normal * (Box.GetCenter().Z - Box.Min.Z);
	const FQuat AlignmentQuat = FRotationMatrix::MakeFromZ(Hit.Normal).ToQuat();
	const FQuat TurnQuat(FVector::UpVector, FMath::DegreesToRadians(SavedPrototypeRotation));
	
	Cell->SetActorLocationAndRotation(Location, AlignmentQuat * TurnQuat);
}

bool AAbyssPlayerController::ConstructCellFromPrototype(ACell*& Prototype)
{
	if (!Prototype->CanGoBackFromPrototype())
		return false;

	Prototype->ChangeBackToNormal();
	Prototype = nullptr;
	return true;
}






//construction-related handling

void AAbyssPlayerController::HandleConstructionModeToggleInput(const FInputActionValue& Value)
{
	if (CurrentInteractionMode == EInteractionMode::Construction)
		CurrentInteractionMode = EInteractionMode::No;
	else
		CurrentInteractionMode = EInteractionMode::Construction;
	
	ResetCellPrototype();
}

void AAbyssPlayerController::HandleSelectedCellChangeInput(const FInputActionValue& Value)
{
	if (CurrentInteractionMode != EInteractionMode::Construction)
		return;
	
	SelectedConstructionCellIndex = (SelectedConstructionCellIndex + 1) % ConstructableCells.Num();
	ResetCellPrototype();

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan,
		FString("You've picked ") + ConstructableCells[SelectedConstructionCellIndex]->GetName());
}

void AAbyssPlayerController::HandleCellRotationInput(const FInputActionValue& Value)
{
	if (!PrototypeCell)
		return;
	
	SavedPrototypeRotation += Value.Get<float>() * RotationSpeed * GetWorld()->DeltaTimeSeconds;
}




//mouse & look

void AAbyssPlayerController::HandleInteractInput(const FInputActionValue& Value)
{
	EnableImmersiveMode();

	if (bInLook)
		return;

	const bool Pressed = Value.Get<float>() > KINDA_SMALL_NUMBER;
	
	if (CurrentInteractionMode == EInteractionMode::Construction && Pressed
		&& ConstructCellFromPrototype(PrototypeCell))
	{
		ResetCellPrototype();
	}
	else if (CurrentInteractionMode == EInteractionMode::No && Pressed)
	{
		FVector2f ScreenPos;
		GetMousePosition(ScreenPos.X, ScreenPos.Y);
		FHitResult BodyHit;
		TraceAtScreenPos(BodyHit, ECollisionChannel::ECC_PhysicsBody, ScreenPos);
		DraggedItem = Cast<AItem>(BodyHit.GetActor());
		CurrentInteractionMode = EInteractionMode::ItemDrag;
	}
	else if (CurrentInteractionMode == EInteractionMode::ItemDrag && !Pressed)
	{
		DraggedItem = nullptr;
		CurrentInteractionMode = EInteractionMode::No;
	}
}

void AAbyssPlayerController::HandleCursorPosChangeInput(const FInputActionValue& Value)
{
	if(bInLook)
	{
		const float dt = GetWorld()->GetDeltaSeconds();
		AddYawInput(Value[0] * dt * LookRate);
		AddPitchInput(Value[1] * dt * LookRate);
	}
}

void AAbyssPlayerController::HandleTurnChangeInput(const FInputActionValue& Value)
{
	bInLook = Value.Get<float>() > KINDA_SMALL_NUMBER;

	SetShowMouseCursor(!bInLook);
	EnableImmersiveMode();
	
	if (bInLook)
		GetMousePosition(SavedInteractionCursorPos.X, SavedInteractionCursorPos.Y);
	else
		SetMouseLocation(SavedInteractionCursorPos.X, SavedInteractionCursorPos.Y);
}



//motion
void AAbyssPlayerController::HandleSpeedBoostInput(const FInputActionValue& Value)
{
	const bool bBoosted = Value.Get<float>() > KINDA_SMALL_NUMBER;
	if (AbyssPawn)
		AbyssPawn->ToggleBoostedSpeed(bBoosted);
}

void AAbyssPlayerController::HandleDirectMotionInput(const FInputActionValue& Value)
{
		
	if (AbyssPawn)
		AbyssPawn->MoveForward_Horizontal(Value.Get<float>());
}

void AAbyssPlayerController::HandleSideMotionInput(const FInputActionValue& Value)
{
	if (AbyssPawn)
		AbyssPawn->MoveRight_Horizontal(Value.Get<float>());
}

void AAbyssPlayerController::HandleVerticalMotionInput(const FInputActionValue& Value)
{
	if (AbyssPawn)
		AbyssPawn->MoveUp_Vertical(Value.Get<float>());
}



