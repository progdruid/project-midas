// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class PROJECTMIDAS_API AItem : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FName Name;

private:
	UPROPERTY(Transient)
	UPrimitiveComponent* Body;

public:	
	AItem();
	
protected:
	virtual void PostInitializeComponents() override;
	
public:
	UFUNCTION(BlueprintCallable)
	void ApplyAcceleration (FVector Acc);
	UFUNCTION(BlueprintCallable)
	void ApplySweep (FVector Sweep, float SpeedLimit);
	UFUNCTION(BlueprintCallable)
	void ToggleGravity(bool Value);
	UFUNCTION(BlueprintCallable)
	void SetVelocity(FVector Vel);
	UFUNCTION(BlueprintCallable)
	void AddVelocity(FVector Vel);
	virtual FVector GetVelocity() const override;
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Price;
};
