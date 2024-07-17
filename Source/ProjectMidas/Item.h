// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class PROJECTMIDAS_API AItem : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FName Name;

private:
	UPROPERTY(Transient)
	UPrimitiveComponent* Body;

	// UPROPERTY(Transient)
	// TArray<UPrimitiveComponent*> TouchingComponents;
	
public:	
	AItem();
	
protected:
	virtual void PostInitializeComponents() override;
	// virtual void Tick(float DeltaSeconds) override;
	
public:
	UFUNCTION(BlueprintCallable)
	void ApplyAcceleration (FVector Acc);
	UFUNCTION(BlueprintCallable)
	void ToggleGravity(bool Value);
	// UFUNCTION(BlueprintCallable)
	// bool IsTouchingGround () const;

// private:
//  	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
};