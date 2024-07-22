// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cell.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class PROJECTMIDAS_API ACell : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cell")
	FName HitVolumeName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cell")
	bool bSpawnedAsPrototype = true;
	
private:
	UPROPERTY(Transient)
	UPrimitiveComponent* HitVolume;
	FCollisionResponseContainer HitVolumeDefaultResponses;

	UPROPERTY(Transient)
	TMap<UPrimitiveComponent*, TEnumAsByte<ECollisionEnabled::Type>> OtherPrimitivesAndCollision;
	
	
	bool bIsPrototype = false;
	
	
public:	
	ACell();

protected:
	virtual void PostInitializeComponents() override;

public:
	UFUNCTION(BlueprintCallable)
	void AccountPrimitives();
	UFUNCTION(BlueprintCallable)
	void ChangeToPrototype (bool bBack = false);
	UFUNCTION(BlueprintCallable)
	bool CanGoBackFromPrototype() const;

	UFUNCTION(BlueprintCallable)
	bool IsPrototype() const;
};
