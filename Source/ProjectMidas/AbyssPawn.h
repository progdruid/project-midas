// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbyssPawn.generated.h"

class USphereComponent;

UCLASS(Blueprintable, BlueprintType)
class PROJECTMIDAS_API AAbyssPawn : public APawn
{
	GENERATED_BODY()

public:
	AAbyssPawn();
	
protected:
	virtual UPawnMovementComponent* GetMovementComponent() const override
		{ return MovementComponent; }
	virtual void UpdateNavigationRelevance() override;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ColliderSphereRadius = 35.f;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> CollisionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPawnMovementComponent> MovementComponent;

public:
	UFUNCTION(BlueprintCallable, Category="Pawn")
	void MoveForward_Horizontal(float Val);
	UFUNCTION(BlueprintCallable, Category="Pawn")
	void MoveRight_Horizontal(float Val);
	UFUNCTION(BlueprintCallable, Category="Pawn")
	void MoveUp_Vertical(float Val);
};
