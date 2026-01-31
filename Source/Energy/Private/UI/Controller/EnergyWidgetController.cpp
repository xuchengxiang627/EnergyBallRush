// Fill out your copyright notice in the Description page of Project Settings.


#include "Energy/Public/UI/Controller/EnergyWidgetController.h"

#include "Player/EnergyPlayerController.h"
#include "Player/EnergyPlayerState.h"

void UEnergyWidgetController::SetWidgetControllerParams(APlayerController* PC, APlayerState* PS)
{
	PlayerController = PC;
	PlayerState = PS;
}

AEnergyPlayerController* UEnergyWidgetController::GetEnergyPlayerController()
{
	if (EnergyPlayerController == nullptr)
	{
		EnergyPlayerController = Cast<AEnergyPlayerController>(PlayerController);
	}
	return EnergyPlayerController;
}

AEnergyPlayerState* UEnergyWidgetController::GetEnergyPlayerState()
{
	if (EnergyPlayerState == nullptr)
	{
		EnergyPlayerState = Cast<AEnergyPlayerState>(PlayerState);
	}
	return EnergyPlayerState;
}

void UEnergyWidgetController::BroadcastInitialValues()
{
}

void UEnergyWidgetController::BindCallbacksToDependencies()
{
}
