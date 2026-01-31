// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActorEffectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActorEffectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ENERGY_API IActorEffectInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnSpeedupEffect(float NewSpeed, float Duration);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDamageIncreaseEffect(float NewDamageBonus, float Duration);
};
