// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbyssPlayerControllerBase.generated.h"

class AItem;
class AAbyssPawnBase;
/**
 * A player controller for the main abyss map where the whole game takes place
 */
UCLASS(Abstract)
class PROJECTMIDAS_API AAbyssPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	AAbyssPlayerControllerBase() = default;
	
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
	UInputAction* InputLook;
	
private:
	UPROPERTY(Transient)
	AItem* DraggedItem;
	UPROPERTY(Transient)
	AAbyssPawnBase* AbyssPawn;
	UPROPERTY(Transient)
	UEnhancedInputComponent* EIC;
	
private:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void Tick(float DeltaSeconds) override;
	
public:
	UFUNCTION(BlueprintCallable)
	void ChangeItemDrag(bool bInDrag);

private:
	bool TraceForward(FHitResult& Hit, ECollisionChannel Channel) const;

	void HandleDirectMotionInput (const FInputActionValue& Value);
	void HandleSideMotionInput (const FInputActionValue& Value);
	void HandleVerticalMotionInput (const FInputActionValue& Value);
	void HandleLookInput (const FInputActionValue& Value);
};
