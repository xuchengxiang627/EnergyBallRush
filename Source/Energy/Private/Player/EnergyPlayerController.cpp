// Copyright Epic Games, Inc. All Rights Reserved.


#include "Energy/Public/Player/EnergyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Components/WidgetComponent.h"
#include "Energy/Public/Player/EnergyHUD.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Character.h"
#include "UI/DamageTextComponent.h"

void AEnergyPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		UDamageTextComponent* DamageTextComponent = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageTextComponent->RegisterComponent();
		DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageTextComponent->SetDamageText(DamageAmount);
	}
}

AEnergyPlayerController::AEnergyPlayerController()
{
}

void AEnergyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}
