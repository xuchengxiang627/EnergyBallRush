// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "EnergyGameMode.generated.h"

UCLASS(minimalapi)
class AEnergyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AEnergyGameMode();
	FTimerHandle GameTimeHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	bool bIsGameOver = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	float GameTime = 15.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	float GameRestartTime = 5.f;

	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;

	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	// virtual bool ReadyToStartMatch_Implementation() override;
	virtual bool ReadyToEndMatch_Implementation() override;

	// 游戏计时UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class UUserWidget> GameTimeWidgetClass;

	// 游戏结束UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class UUserWidget> GameOverWidgetClass;
private:
	void GameOver();
	void RestartMap();
};



