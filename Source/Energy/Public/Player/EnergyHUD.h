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
public:
	UPROPERTY()
	TObjectPtr<UOverlayWidget> OverlayWidget;
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	void InitOverlay(APlayerController* PC, APlayerState* PS);

	UOverlayWidgetController* GetOverlayWidgetController(APlayerController* PC, APlayerState* PS);
};
