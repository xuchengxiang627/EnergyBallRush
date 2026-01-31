// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnergyBall.generated.h"

class AEnergyCharacter;
class USphereComponent;

UCLASS()
class ENERGY_API AEnergyBall : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category=EnergyBall)
	TObjectPtr<USphereComponent> CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category=EnergyBall)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=EnergyBall)
	float HiddenTime = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=EnergyBall)
	TObjectPtr<USoundBase> PickupSound;

	AEnergyBall();

	FTimerHandle HiddenTimerHandle;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void EffectToCharacter(AEnergyCharacter* EnergyCharacter);
protected:
	virtual void BeginPlay() override;

	void Hide();
	void Show();

	UFUNCTION(Client, Unreliable)
	void ClientPickup();

private:
	UFUNCTION(Server, Reliable)
	void AddScore(AEnergyCharacter* EnergyCharacter, int32 Score);
};


