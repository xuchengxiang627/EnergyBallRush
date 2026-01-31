// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnergyAIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class ENERGY_API AEnergyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEnergyAIController();
protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
