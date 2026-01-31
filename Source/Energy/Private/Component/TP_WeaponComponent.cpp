// Copyright Epic Games, Inc. All Rights Reserved.


#include "Energy/Public/Component/TP_WeaponComponent.h"
#include "Energy/Public/Character/EnergyCharacter.h"
#include "Energy/Public/Actor/EnergyProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Player/EnergyHUD.h"
#include "Player/EnergyPlayerController.h"
#include "Player/EnergyPlayerState.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UTP_WeaponComponent::Fire()
{
	ServerFire();
}

void UTP_WeaponComponent::EndFire()
{
	if (FireTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimer);
	}

}

bool UTP_WeaponComponent::AttachWeapon(AEnergyCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	// FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));
	// AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);
	SetIsReplicated(true);
	SetSimulatePhysics(false);
	SetEnableGravity(false);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
			// EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UTP_WeaponComponent::EndFire);
		}
	}

	if (AEnergyPlayerState* EnergyPlayerState = Character->GetPlayerState<AEnergyPlayerState>())
	{
		EnergyPlayerState->SetHoldWeapon(true);
	}


	return true;
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

void UTP_WeaponComponent::MultiClientFire_Implementation()
{
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance(); AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::ServerFire_Implementation()
{
	if (Character == nullptr || Character->GetController() == nullptr || GetWorld()->GetTimerManager().IsTimerActive(FireTimer))
	{
		return;
	}
	if (UWorld* const World = GetWorld(); World != nullptr && !World->GetTimerManager().IsTimerActive(FireTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(FireTimer, FireRate, false);
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			FHitResult HitResult;
			FVector Direction = PlayerController->PlayerCameraManager->GetCameraRotation().Vector();
			const FVector Start = Character->GetFirstPersonCameraComponent()->GetComponentLocation();
			const FVector End = Start + Direction * FireDistance;



			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(Character);
			World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);
			// 画出检测线
			DrawDebugLine(World, Start, HitResult.ImpactPoint, FColor::Red, false, 1.f, 0, 1.f);
			if (HitResult.bBlockingHit)
			{
				if (HitResult.GetActor()->Implements<UCombatInterface>())
				{
					const float Damage = ICombatInterface::Execute_GetDamageBonus(Character) * FireDamage;
					ICombatInterface::Execute_AddHealth(HitResult.GetActor(), -Damage);
					if (AEnergyPlayerController* EnergyPlayerController = Cast<AEnergyPlayerController>(PlayerController))
					{
						EnergyPlayerController->ShowDamageNumber(Damage, Cast<ACharacter>(HitResult.GetActor()));
					}
					if (ICombatInterface::Execute_GetHealth(HitResult.GetActor()) <= 0)
					{
						ICombatInterface::Execute_AddEnergyScore(Character, 10);
						// Cast<ICombatInterface>(HitResult.GetActor())->Die();
					}
				}
			}
			MultiClientFire();
		}
	}
}

void UTP_WeaponComponent::DetachWeapon()
{
	// 清除定时器
	if (FireTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimer);
	}

	// 从父组件分离
	DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	SetSimulatePhysics(true);
	SetEnableGravity(true);

	// 从角色移除实例组件
	if (Character)
	{
		Character->RemoveInstanceComponent(this);
	}
	EndPlay(EEndPlayReason::Type::Destroyed);
	// 清空角色引用
	Character = nullptr;
}
