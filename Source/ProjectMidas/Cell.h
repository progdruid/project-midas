// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cell.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCellEvent);

UCLASS(Abstract, Blueprintable, BlueprintType)
class PROJECTMIDAS_API ACell : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cell")
	FName HitVolumeName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cell")
	bool bSpawnedAsPrototype = false;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FCellEvent OnCellStart;
	UPROPERTY(BlueprintAssignable, Category="Events")
	FCellEvent OnCellEnd;
	
	
private:
	UPROPERTY(Transient)
	UPrimitiveComponent* HitVolume;
	ECollisionEnabled::Type HitVolumeDefaultCollisionEnabled;
	ECollisionChannel HitVolumeDefaultObjectType;
	FCollisionResponseContainer HitVolumeDefaultResponses;

	UPROPERTY(Transient)
	TMap<UPrimitiveComponent*, TEnumAsByte<ECollisionEnabled::Type>> OtherPrimitivesAndCollision;
	
	
	bool bIsPrototype = false;
	
	
public:	
	ACell();

protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(BlueprintCallable)
	void AccountPrimitives();
	UFUNCTION(BlueprintCallable)
	void ChangeToPrototype ();
	UFUNCTION(BlueprintCallable)
	void ChangeBackToNormal ();
	UFUNCTION(BlueprintCallable)
	bool CanGoBackFromPrototype() const;

	UFUNCTION(BlueprintCallable)
	bool IsPrototype() const;
};
