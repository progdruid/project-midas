// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GameFramework/PlayerController.h"
#include "AbyssPlayerControllerBase.generated.h"

/**
 * A player controller for the main abyss map where the whole game takes place
 */
UCLASS(Abstract)
class PROJECTMIDAS_API AAbyssPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	AAbyssPlayerControllerBase();
	
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int TraceDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DragAccelerationCoefficient;
	
private:
	AItem* DraggedItem;
	
public:
	UFUNCTION(BlueprintCallable)
	void ChangeItemDrag(bool bInDrag);

	
private:
	bool TraceForward(FHitResult& Hit, ECollisionChannel Channel) const;
	
};
