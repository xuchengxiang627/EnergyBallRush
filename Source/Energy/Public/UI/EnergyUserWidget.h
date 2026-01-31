// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnergyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ENERGY_API UEnergyUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UObject> WidgetController;

	void SetWidgetController(UObject* InWidgetController);
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
