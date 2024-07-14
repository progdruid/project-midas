// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "ItemDetectorComponent.generated.h"

class AItem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemDetectionEvent, AItem*, TriggeredItem);

/**
 * A volume detector for items 
 */
UCLASS(Blueprintable,
	BlueprintType, meta=(BlueprintSpawnableComponent),
	HideCategories=
		(Replication, ComponentReplication, RayTracing, HLOD, Variable,
		Navigation, ComponentTick, Rendering, PathTracing, Physics, Collision,
		Tags, Cooking, Events, Mobile, AssetUserData, Shape),
		ShowCategories=(Shape))
class PROJECTMIDAS_API UItemDetectorComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FItemDetectionEvent OnItemEnterEvent;
	UPROPERTY(BlueprintAssignable, Category="Events")
	FItemDetectionEvent OnItemExitEvent;

protected:
	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<AItem*> OverlappingItems;
	
public:
	UItemDetectorComponent();
	
private:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult & SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherCom, int32 OtherBodyIndex);

public:
	UFUNCTION(BlueprintGetter)
	TArray<AItem*>& GetItemsInsideCopy ();
	const TArray<AItem*>& GetItemsInside () const;
};
