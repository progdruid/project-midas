// Fill out your copyright notice in the Description page of Project Settings.


#include "AbyssPlayerState.h"

AAbyssPlayerState::AAbyssPlayerState()
{
}

float AAbyssPlayerState::GetCurrentMoney() const
{
	return MoneyAmount;
}

bool AAbyssPlayerState::HasAmountMoney(float Amount) const
{
	return MoneyAmount > Amount;
}

void AAbyssPlayerState::WithdrawMoney(float Amount)
{
	MoneyAmount -= Amount;
	PrintMoney();
}

bool AAbyssPlayerState::TryWithdrawMoney(float Amount)
{
	if (!HasAmountMoney(Amount))
		return false;
	
	WithdrawMoney(Amount);
	return true;
}

void AAbyssPlayerState::DepositMoney(float Amount)
{
	MoneyAmount += Amount;
	PrintMoney();
}

void AAbyssPlayerState::PrintMoney() const
{
	GEngine->RemoveOnScreenDebugMessage(1001);
	GEngine->AddOnScreenDebugMessage(1001, 10.f, FColor::Green,
		FString("Current money: ") + FString::SanitizeFloat(MoneyAmount), false);
}

