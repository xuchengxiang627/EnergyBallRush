// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EnergyPlayerState.h"

#include "Net/UnrealNetwork.h"

AEnergyPlayerState::AEnergyPlayerState()
{
	DamageBonus = BaseDamageBonus;
	Speed = BaseSpeed;
}

void AEnergyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEnergyPlayerState, Health);
	DOREPLIFETIME(AEnergyPlayerState, MaxHealth);
	DOREPLIFETIME(AEnergyPlayerState, DamageBonus);
	DOREPLIFETIME(AEnergyPlayerState, BaseDamageBonus);
	DOREPLIFETIME(AEnergyPlayerState, BaseSpeed);
	DOREPLIFETIME(AEnergyPlayerState, Speed);
	DOREPLIFETIME(AEnergyPlayerState, EnergyScore);
	DOREPLIFETIME(AEnergyPlayerState, bHoldWeapon);
}

/** Replication */
void AEnergyPlayerState::OnRep_Health(float OldHealth)
{
	OnHealthChange.Broadcast(Health);
}

void AEnergyPlayerState::OnRep_MaxHealth(float OldMaxHealth)
{
	OnMaxHealthChange.Broadcast(MaxHealth);
}

void AEnergyPlayerState::OnRep_DamageBonus(float OldDamageBonus)
{
	OnDamageBonusChange.Broadcast(DamageBonus);
}

void AEnergyPlayerState::OnRep_BaseDamageBonus(float OldBaseDamageBonus)
{
	OnBaseDamageBonusChange.Broadcast(BaseDamageBonus);
}

void AEnergyPlayerState::OnRep_BaseSpeed(float OldBaseSpeed)
{
	OnBaseSpeedChange.Broadcast(BaseSpeed);
}

void AEnergyPlayerState::OnRep_Speed(float OldSpeed)
{
	OnSpeedChange.Broadcast(Speed);
}

void AEnergyPlayerState::OnRep_EnergyScore(float OldScore)
{
	OnEnergyScoreChange.Broadcast(EnergyScore);
}

void AEnergyPlayerState::OnRep_HoldWeapon(bool OldHoldWeapon)
{
	OnHoldWeaponChange.Broadcast(bHoldWeapon);
}

/**---------------- Set -----------------------------------*/
void AEnergyPlayerState::SetHealth(float NewHealth)
{
	Health = NewHealth;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
	OnHealthChange.Broadcast(Health);
}

void AEnergyPlayerState::SetMaxHealth(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
	OnMaxHealthChange.Broadcast(MaxHealth);
}

void AEnergyPlayerState::SetDamageBonus(float NewDamageBonus)
{
	DamageBonus = NewDamageBonus;
	OnDamageBonusChange.Broadcast(DamageBonus);
}

void AEnergyPlayerState::SetBaseDamageBonus(float NewBaseDamageBonus)
{
	BaseDamageBonus = NewBaseDamageBonus;
	OnBaseDamageBonusChange.Broadcast(BaseDamageBonus);
}

void AEnergyPlayerState::SetBaseSpeed(float NewBaseSpeed)
{
	BaseSpeed = NewBaseSpeed;
	OnBaseSpeedChange.Broadcast(BaseSpeed);
}

void AEnergyPlayerState::SetSpeed(float NewSpeed)
{
	Speed = NewSpeed;
	OnSpeedChange.Broadcast(Speed);
}

void AEnergyPlayerState::SetEnergyScore(float NewScore)
{
	EnergyScore = NewScore;
	OnEnergyScoreChange.Broadcast(EnergyScore);
}

void AEnergyPlayerState::SetHoldWeapon(bool IsHoldWeapon)
{
	bHoldWeapon = IsHoldWeapon;
	OnHoldWeaponChange.Broadcast(bHoldWeapon);
}

/**----------------- Add --------------------------------*/
void AEnergyPlayerState::AddHealth(float InHealth)
{
	Health += InHealth;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
	OnHealthChange.Broadcast(Health);
}

void AEnergyPlayerState::AddMaxHealth(float InMaxHealth)
{
	MaxHealth += InMaxHealth;
	OnMaxHealthChange.Broadcast(MaxHealth);
}

void AEnergyPlayerState::AddDamageBonus(float InDamageBonus)
{
	DamageBonus += InDamageBonus;
	OnDamageBonusChange.Broadcast(DamageBonus);
}

void AEnergyPlayerState::AddBaseDamageBonus(float InBaseDamageBonus)
{
	BaseDamageBonus += InBaseDamageBonus;
	OnBaseDamageBonusChange.Broadcast(BaseDamageBonus);
}

void AEnergyPlayerState::AddBaseSpeed(float InBaseSpeed)
{
	BaseSpeed += InBaseSpeed;
	OnSpeedChange.Broadcast(BaseSpeed);
}

void AEnergyPlayerState::AddSpeed(float InSpeed)
{
	Speed += InSpeed;
	OnSpeedChange.Broadcast(Speed);
}

void AEnergyPlayerState::AddEnergyScore(float InScore)
{
	EnergyScore += InScore;
	OnEnergyScoreChange.Broadcast(EnergyScore);
}
