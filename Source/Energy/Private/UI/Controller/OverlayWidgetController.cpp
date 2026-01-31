// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Controller/OverlayWidgetController.h"

#include "Player/EnergyPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	AEnergyPlayerState* PS = GetEnergyPlayerState();
	if (PS != nullptr)
	{
		OnHealthChanged.Broadcast(PS->GetHealth());
		OnMaxHealthChanged.Broadcast(PS->GetMaxHealth());
		OnScoreChanged.Broadcast(PS->GetEnergyScore());
		OnHoldWeaponChanged.Broadcast(PS->IsHoldWeapon());
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AEnergyPlayerState* PS = GetEnergyPlayerState();
	if (PS != nullptr)
	{
		PS->OnHealthChange.AddLambda([&](float NewHealth)
		{
			OnHealthChanged.Broadcast(NewHealth);
		});
		PS->OnMaxHealthChange.AddLambda([&](float NewMaxHealth)
		{
			OnMaxHealthChanged.Broadcast(NewMaxHealth);
		});
		PS->OnEnergyScoreChange.AddLambda([&](float NewScore)
		{
			OnScoreChanged.Broadcast(NewScore);
		});
		PS->OnHoldWeaponChange.AddLambda([&](bool NewHoldWeapon)
		{
			OnHoldWeaponChanged.Broadcast(NewHoldWeapon);
		});
	}
}
