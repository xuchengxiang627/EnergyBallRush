// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnergyWidgetController.generated.h"

class AEnergyPlayerController;
class AEnergyPlayerState;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ENERGY_API UEnergyWidgetController : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<APlayerState> PlayerState;

	void SetWidgetControllerParams(APlayerController* PC, APlayerState* PS);

	AEnergyPlayerController* GetEnergyPlayerController();
	AEnergyPlayerState* GetEnergyPlayerState();

	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<AEnergyPlayerController> EnergyPlayerController;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<AEnergyPlayerState> EnergyPlayerState;
};
