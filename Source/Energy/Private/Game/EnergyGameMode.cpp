// Copyright Epic Games, Inc. All Rights Reserved.

#include "Energy/Public/Game/EnergyGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Energy/Public/Character/EnergyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

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
	UUserWidget* GameTimeWidget = CreateWidget<UUserWidget>(GetWorld(), GameTimeWidgetClass);
	if (GameTimeWidget)
	{
		GameTimeWidget->AddToViewport();
	}
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

void AEnergyGameMode::GameOver()
{
	bIsGameOver = true;
}

void AEnergyGameMode::RestartMap()
{
	GetWorld()->ServerTravel(GetWorld()->GetName(), false, false);
}
