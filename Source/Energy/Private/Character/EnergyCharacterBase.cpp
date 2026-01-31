// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnergyCharacterBase.h"


void AEnergyCharacterBase::InitialCharacter()
{
}

// Sets default values
AEnergyCharacterBase::AEnergyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnergyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnergyCharacterBase::Die()
{
	// TODO: 死亡处理
	UE_LOG(LogTemp, Warning, TEXT("%s died"), *GetName());
}

