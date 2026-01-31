// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "EnergyGameState.generated.h"

/**
 * 
 */
UCLASS()
class ENERGY_API AEnergyGameState : public AGameState
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category="Gameplay")
	float GameTime = 15.f;
public:
	UFUNCTION(BlueprintPure, Category = "Game State")
	float GetGameTime() const { return GameTime; }
	// 判断本地玩家是否为胜者
	UFUNCTION(BlueprintPure, Category = "Game State")
	bool IsLocalPlayerWinner() const;

};
