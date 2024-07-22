// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbyssPlayerController.generated.h"

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Default")
	int TraceDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Default")
	float DragAccelerationCoefficient;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Default")
	float LookRate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* InputDirectHorizontal;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* InputSideHorizontal;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* InputVertical;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* InputLookMotion;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* InputTurnChange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* InputInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* InputConstructionToggle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Default")
	TArray<TSubclassOf<ACell>> ConstructableCells;
	
private:
	EInteractionMode CurrentInteractionMode = EInteractionMode::No;
	
	UPROPERTY(Transient)
	AItem* DraggedItem;
	UPROPERTY(Transient)
	AAbyssPawn* AbyssPawn;

	UPROPERTY(Transient)
	UEnhancedInputComponent* EIC;

	UPROPERTY(Transient)
	UGameViewportClient* GameViewportClient;
	FReply* SlateOperations;
	TShaderRef<SViewport> ViewportRef;
	
	FVector2f SavedInteractionCursorPos;

	bool bInLook = false;

	FVector DraggedItemTarget;

	bool bInConstructionMode = false;
	int SelectedConstructionCellIndex = 0;
	UPROPERTY(Transient)
	ACell* PrototypeCell;
	
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
	
	void ResetCellPrototype ();
	void PlaceCellPrototypeAtHit(ACell* Cell, const FHitResult& Hit) const;
	void ConstructCellFromPrototype (ACell*& Prototype);

	void HandleConstructionModeToggle (const FInputActionValue& Value);

	//mouse & look
	void HandleInteractInput (const FInputActionValue& Value);
	void HandleCursorPosChange (const FInputActionValue& Value);
	void HandleTurnChangeInput (const FInputActionValue& Value);

	//motion
	void HandleDirectMotionInput (const FInputActionValue& Value);
	void HandleSideMotionInput (const FInputActionValue& Value);
	void HandleVerticalMotionInput (const FInputActionValue& Value);

};
