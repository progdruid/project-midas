// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbyssPlayerController.generated.h"

class AAbyssPlayerState;
class ACell;
class AAbyssPawn;
class AItem;
class FReply;


enum class EInteractionMode
{
	No,
	ItemDrag,
	Construction
};


/**
 * A player controller for the main abyss map where the whole game takes place
 */
UCLASS(Abstract)
class PROJECTMIDAS_API AAbyssPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* InputDirectHorizontal;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* InputSideHorizontal;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* InputVertical;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* InputSpeedBoost;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* InputLookMotion;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* InputTurnChange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* InputInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* InputConstructionToggle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* InputSelectedCellChange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* InputCellRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Default")
	int TraceDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Default")
	float DragAccelerationCoefficient;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Default")
	float LookRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Default")
	float RotationSpeed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Default")
	TArray<TSubclassOf<ACell>> ConstructableCells;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Default")
	UDataTable* CellDataTable;
	
private:
	//saved stuff
	UPROPERTY(Transient)
	UEnhancedInputComponent* EIC;

	UPROPERTY(Transient)
	UGameViewportClient* GameViewportClient;
	FReply* SlateOperations;
	TShaderRef<SViewport> ViewportRef;
	
	UPROPERTY(Transient)
	AAbyssPlayerState* AbyssPlayerState;
	UPROPERTY(Transient)
	AAbyssPawn* AbyssPawn;

	
	//drag stuff
	UPROPERTY(Transient)
	AItem* DraggedItem;

	//construction stuff
	UPROPERTY(Transient)
	ACell* PrototypeCell;
	int ConstructedCellIndex = 0;
	float SavedPrototypeRotation = 0.f;
	bool bInvertRotation = false;

	FVector2f SavedInteractionCursorPos;
	bool bInLook = false;
	EInteractionMode CurrentInteractionMode = EInteractionMode::No;
	
public:
	AAbyssPlayerController() = default;
	
private:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	bool TraceAtScreenPos(FHitResult& Hit, ECollisionChannel Channel, const FVector2f& ScreenPos) const;
	FVector2f GetCurrentInteractionCursorPosition () const;
	void EnableImmersiveMode();

	//construction implementation
	void ResetCellPrototype ();
	void PlaceCellPrototypeAtHit(ACell* Cell, const FHitResult& Hit) const;
	bool ConstructCellFromPrototype(ACell*& Prototype);

	//construction-related handling
	void HandleConstructionModeToggleInput (const FInputActionValue& Value);
	void HandleSelectedCellChangeInput (const FInputActionValue& Value);
	void HandleCellRotationInput (const FInputActionValue& Value);
	
	//mouse & look
	void HandleInteractInput (const FInputActionValue& Value);
	void HandleCursorPosChangeInput (const FInputActionValue& Value);
	void HandleTurnChangeInput (const FInputActionValue& Value);

	//motion
	void HandleSpeedBoostInput (const FInputActionValue& Value);
	void HandleDirectMotionInput (const FInputActionValue& Value);
	void HandleSideMotionInput (const FInputActionValue& Value);
	void HandleVerticalMotionInput (const FInputActionValue& Value);

};
