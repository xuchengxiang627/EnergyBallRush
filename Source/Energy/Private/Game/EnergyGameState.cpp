// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/EnergyGameState.h"

#include "GameFramework/PlayerState.h"
#include "Player/EnergyPlayerState.h"

bool AEnergyGameState::IsLocalPlayerWinner() const
{
	if (!GetWorld()) return false;

	// 获取本地玩家控制器
	APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();
	if (!LocalPlayerController) return false;

	// 获取本地玩家的 PlayerState
	AEnergyPlayerState* LocalPlayerState = LocalPlayerController->GetPlayerState<AEnergyPlayerState>();
	if (!LocalPlayerState) return false;

	// 获取本地玩家的分数
	int32 LocalPlayerScore = LocalPlayerState->GetEnergyScore();

	// 获取所有玩家状态并比较分数
	for (APlayerState* PlayerState : PlayerArray)
	{
		AEnergyPlayerState* EnergyPlayerState = Cast<AEnergyPlayerState>(PlayerState);
		if (EnergyPlayerState && EnergyPlayerState->GetEnergyScore() > LocalPlayerScore)
		{
			return false; // 发现比本地玩家分数更高的玩家
		}
	}

	return true;
}
