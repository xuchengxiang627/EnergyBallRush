// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EnergyHUD.generated.h"

class UOverlayWidget;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class ENERGY_API AEnergyHUD : public AHUD
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UOverlayWidget> OverlayWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	// 游戏计时UI
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> GameTimeWidgetClass;

	// 游戏结束UI
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
public:
	UPROPERTY()
	TObjectPtr<UOverlayWidget> OverlayWidget;
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	void InitOverlay(APlayerController* PC, APlayerState* PS);
	void CreateGameTimeWidget();
	UFUNCTION(BlueprintCallable)
	void CreateGameOverWidget();

	UOverlayWidgetController* GetOverlayWidgetController(APlayerController* PC, APlayerState* PS);
};
