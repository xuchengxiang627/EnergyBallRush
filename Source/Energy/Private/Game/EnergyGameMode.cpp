// Copyright Epic Games, Inc. All Rights Reserved.

#include "Energy/Public/Game/EnergyGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Energy/Public/Character/EnergyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"

AEnergyGameMode::AEnergyGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

bool AEnergyGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	return false;
}

void AEnergyGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	GetWorldTimerManager().SetTimer(GameTimeHandle, this, &AEnergyGameMode::GameOver, GameTime, false);
}

void AEnergyGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	TArray<AActor*> Characters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnergyCharacter::StaticClass(), Characters);
	for (AActor* Character : Characters)
	{
		Character->Destroy();
	}
	// 创建一个游戏结束的UI
	UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
	if (GameOverWidget)
	{
		GameOverWidget->AddToViewport();
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnergyGameMode::RestartMap, GameRestartTime, false);
}

bool AEnergyGameMode::ReadyToEndMatch_Implementation()
{
	return bIsGameOver;
}

void AEnergyGameMode::MultiClientCreateUI_Implementation(TSubclassOf<UUserWidget> WidgetClass)
{
	UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	if (GameOverWidget)
	{
		GameOverWidget->AddToViewport();
	}
}

void AEnergyGameMode::GameOver()
{
	bIsGameOver = true;
}

void AEnergyGameMode::RestartMap()
{
	GetWorld()->ServerTravel(GetWorld()->GetName(), false, false);
}

void AEnergyGameMode::HandlePlayerDied(AController* Controller)
{
	// 等待一段时间后重生玩家
	FTimerHandle RespawnTimer;
	float RespawnDelay = 3.0f; // 3秒后重生
	
	GetWorldTimerManager().SetTimer(RespawnTimer, [this, Controller]() {
		RestartPlayer(Controller);
	}, RespawnDelay, false);
}

APlayerController* AEnergyGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
	const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* StartPawn = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	AController* NewController = Cast<AController>(StartPawn);
	if (NewController)
	{
		PlayerRespawnMap.Add(NewController, NextRespawnIndex++);
	}

	return StartPawn;
}

AActor* AEnergyGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	// 为每个玩家找到一个合适的出生点
	TArray<AActor*> PlayerStarts;
	if (PlayerStarts.Num() == 0)
	{
		// 如果没有找到出生点，回退到父类方法
		return Super::FindPlayerStart_Implementation(Player, IncomingName);
	}
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	// 根据玩家数量分配出生点
	int32 StartIndex = PlayerRespawnMap.FindRef(Player) % PlayerStarts.Num();
	return PlayerStarts[StartIndex];
}
