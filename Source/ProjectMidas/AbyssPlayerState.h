// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbyssPlayerState.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTMIDAS_API AAbyssPlayerState : public APlayerState
{
	GENERATED_BODY()

	float MoneyAmount = 0.f;
	
public:
	AAbyssPlayerState();

	UFUNCTION(BlueprintCallable)
	void WithdrawMoney(float Amount);
	UFUNCTION(BlueprintCallable)
	void DepositMoney(float Amount);
	UFUNCTION(BlueprintCallable)
	bool HasAmountMoney(float Amount) const;
	UFUNCTION(BlueprintCallable)
	float GetCurrentMoney () const;
};
