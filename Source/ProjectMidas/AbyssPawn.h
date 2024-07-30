// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"
#include "AbyssPawn.generated.h"

class USphereComponent;

UCLASS(Abstract, Blueprintable, BlueprintType)
class PROJECTMIDAS_API AAbyssPawn : public APawn
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pawn")
	float NormalSpeed = 3000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pawn")
	float BoostedSpeed = 9000.f;
	
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> CollisionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UFloatingPawnMovement> MovementComponent;
	UPROPERTY(Transient)
	TObjectPtr<UWorld> World;

public:
	AAbyssPawn();
	
protected:
	virtual void BeginPlay() override;
	virtual UPawnMovementComponent* GetMovementComponent() const override
		{ return MovementComponent.Get(); }
	virtual void UpdateNavigationRelevance() override;

public:
	UFUNCTION(BlueprintCallable, Category="Pawn")
	void ToggleBoostedSpeed (bool Val);
	
	UFUNCTION(BlueprintCallable, Category="Pawn")
	void MoveForward_Horizontal(float Val);
	UFUNCTION(BlueprintCallable, Category="Pawn")
	void MoveRight_Horizontal(float Val);
	UFUNCTION(BlueprintCallable, Category="Pawn")
	void MoveUp_Vertical(float Val);
};
