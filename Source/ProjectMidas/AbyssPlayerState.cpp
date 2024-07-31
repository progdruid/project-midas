// Fill out your copyright notice in the Description page of Project Settings.


#include "AbyssPlayerState.h"

AAbyssPlayerState::AAbyssPlayerState()
{
}

void AAbyssPlayerState::WithdrawMoney(float Amount)
{
	MoneyAmount -= Amount;
}

void AAbyssPlayerState::DepositMoney(float Amount)
{
	MoneyAmount += Amount;
}

bool AAbyssPlayerState::HasAmountMoney(float Amount) const
{
	return MoneyAmount > Amount;
}

float AAbyssPlayerState::GetCurrentMoney() const
{
	return MoneyAmount;
}

