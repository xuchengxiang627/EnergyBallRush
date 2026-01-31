// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "EnergyCharacterBase.generated.h"

UCLASS()
class ENERGY_API AEnergyCharacterBase : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

	virtual void InitialCharacter();
public:
	// Sets default values for this character's properties
	AEnergyCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Die() override;

};
