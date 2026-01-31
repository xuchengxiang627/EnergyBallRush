// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ENERGY_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetHealth();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetMaxHealth();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetDamage();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetDamageBonus();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetBaseDamageBonus();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetSpeed();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetBaseSpeed();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetEnergyScore();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetHealth(float NewHealth);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMaxHealth(float NewMaxHealth);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetDamage(float NewDamage);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetDamageBonus(float NewDamageBonus);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetBaseDamageBonus(float NewBaseDamageBonus);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetSpeed(float NewSpeed);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetBaseSpeed(float NewBaseSpeed);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetEnergyScore(float NewScore);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddHealth(float InHealth);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddMaxHealth(float InMaxHealth);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddDamage(float InDamage);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddDamageBonus(float InDamageBonus);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddBaseDamageBonus(float InBaseDamageBonus);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddSpeed(float InSpeed);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddBaseSpeed(float InBaseSpeed);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddEnergyScore(float InScore);

	virtual void Die() = 0;
};
