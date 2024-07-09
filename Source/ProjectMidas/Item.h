// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class PROJECTMIDAS_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FName Name;

private:
	UPROPERTY(Transient)
	UPrimitiveComponent* Body;
	
public:
	UFUNCTION(BlueprintCallable)
	void ApplyAcceleration (FVector Acc);
	
protected:
	virtual void PostInitializeComponents() override;
};