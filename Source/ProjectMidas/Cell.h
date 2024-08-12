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
	FName Name;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cell")
	FName HitVolumeName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cell")
	TArray<FName> IntactVolumeNames;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cell")
	bool bSpawnedAsPrototype = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cell")
	bool bSurfaceRelativePlacement = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cell")
	bool bIndestructible = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cell")
	bool bTickWhileCell = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cell")
	bool bTickWhilePrototype = false;
	
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

	UFUNCTION(BlueprintImplementableEvent)
	void CellStart ();
	UFUNCTION(BlueprintImplementableEvent)
	void CellEnd ();
	
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
	UFUNCTION(BlueprintNativeEvent)
	bool AreRequirementsMet ();
};

USTRUCT(BlueprintType)
struct FCellData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BuildingCost;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float OperationCost;
	
};
