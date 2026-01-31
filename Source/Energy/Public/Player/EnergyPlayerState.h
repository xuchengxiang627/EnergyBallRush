// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Interface/CombatInterface.h"
#include "EnergyPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlaerStateChangeDelegate, float)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHoldWeaponChangeDelegate, bool)
/**
 * 
 */
UCLASS()
class ENERGY_API AEnergyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AEnergyPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_Health)
	float Health = 100.f;
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_MaxHealth)
	float MaxHealth = 100.f;
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_DamageBonus)
	float DamageBonus;
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_BaseDamageBonus)
	float BaseDamageBonus = 1.f;
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_BaseSpeed)
	float BaseSpeed = 600.f;
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_Speed)
	float Speed;
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_EnergyScore)
	float EnergyScore = 0.f;
	UPROPERTY(ReplicatedUsing=OnRep_HoldWeapon)
	bool bHoldWeapon = false;


	FOnPlaerStateChangeDelegate OnHealthChange;
	FOnPlaerStateChangeDelegate OnMaxHealthChange;
	FOnPlaerStateChangeDelegate OnDamageBonusChange;
	FOnPlaerStateChangeDelegate OnBaseDamageBonusChange;
	FOnPlaerStateChangeDelegate OnBaseSpeedChange;
	FOnPlaerStateChangeDelegate OnSpeedChange;
	FOnPlaerStateChangeDelegate OnEnergyScoreChange;
	FOnHoldWeaponChangeDelegate OnHoldWeaponChange;

	UFUNCTION()
	void OnRep_Health(float OldHealth);
	UFUNCTION()
	void OnRep_MaxHealth(float OldMaxHealth);
	UFUNCTION()
	void OnRep_DamageBonus(float OldDamageBonus);
	UFUNCTION()
	void OnRep_BaseDamageBonus(float OldBaseDamageBonus);
	UFUNCTION()
	void OnRep_BaseSpeed(float OldBaseSpeed);
	UFUNCTION()
	void OnRep_Speed(float OldSpeed);
	UFUNCTION()
	void OnRep_EnergyScore(float OldScore);
	UFUNCTION()
	void OnRep_HoldWeapon(bool OldHoldWeapon);


	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetDamageBonus() const { return DamageBonus; }
	FORCEINLINE float GetBaseDamageBonus() const { return BaseDamageBonus; }
	FORCEINLINE float GetBaseSpeed() const { return BaseSpeed; }
	FORCEINLINE float GetSpeed() const { return Speed; }
	FORCEINLINE float GetEnergyScore() const { return EnergyScore; }
	FORCEINLINE bool IsHoldWeapon() const { return bHoldWeapon; }

	void SetHealth(float NewHealth);
	void SetMaxHealth(float NewMaxHealth);
	void SetDamageBonus(float NewDamageBonus);
	void SetBaseDamageBonus(float NewBaseDamageBonus);
	void SetBaseSpeed(float NewBaseSpeed);
	void SetSpeed(float NewSpeed);
	void SetEnergyScore(float NewScore);
	void SetHoldWeapon(bool NewHoldWeapon);

	void AddHealth(float InHealth);
	void AddMaxHealth(float InMaxHealth);
	void AddDamageBonus(float InDamageBonus);
	void AddBaseDamageBonus(float InBaseDamageBonus);
	void AddBaseSpeed(float InBaseSpeed);
	void AddSpeed(float InSpeed);
	void AddEnergyScore(float InScore);
};
