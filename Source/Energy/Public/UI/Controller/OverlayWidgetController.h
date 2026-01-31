// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Controller/EnergyWidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangedDelegate, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoldWeaponChangedDelegate, bool, IsHoldWeapon);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ENERGY_API UOverlayWidgetController : public UEnergyWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnPlayerStateChangedDelegate OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnPlayerStateChangedDelegate OnMaxHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnPlayerStateChangedDelegate OnScoreChanged;
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnHoldWeaponChangedDelegate OnHoldWeaponChanged;
};
