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
	float GameRestartTime = 5.f;

	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;

	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	// virtual bool ReadyToStartMatch_Implementation() override;
	virtual bool ReadyToEndMatch_Implementation() override;

	UFUNCTION(NetMulticast, Reliable)
	void MultiClientCreateUI(TSubclassOf<class UUserWidget> WidgetClass);

	
	// 复活相关
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void HandlePlayerDied(class AController* Controller);

	UPROPERTY(EditDefaultsOnly)
	TMap<AController*, int32> PlayerRespawnMap;
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	int32 NextRespawnIndex = 0;
	virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;
private:
	void GameOver();
	void RestartMap();
};