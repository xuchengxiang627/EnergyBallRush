// Copyright Epic Games, Inc. All Rights Reserved.

#include "Energy/Public/Character/EnergyCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Component/TP_WeaponComponent.h"
#include "Engine/LocalPlayer.h"
#include "Game/EnergyGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/EnergyHUD.h"
#include "Player/EnergyPlayerController.h"
#include "Player/EnergyPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AEnergyCharacter
AEnergyCharacter::AEnergyCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	// FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	// FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	// Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	// Mesh1P->SetOnlyOwnerSee(true);
	// Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	// Mesh1P->bCastDynamicShadow = false;
	// Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	// Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void AEnergyCharacter::InitialCharacter()
{
	if (AEnergyPlayerState* PS = GetPlayerState<AEnergyPlayerState>())
	{
		PS->OnSpeedChange.AddLambda([this](float NewSpeed)
		{
			GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
		});
		GetCharacterMovement()->MaxWalkSpeed = PS->GetSpeed();
		PS->SetHealth(PS->GetMaxHealth());
		PS->SetDamageBonus(PS->GetBaseDamageBonus());
	}

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);

	if (AEnergyPlayerController* PC = Cast<AEnergyPlayerController>(GetController()))
	{
		if (AEnergyHUD* HUD = PC->GetHUD<AEnergyHUD>())
		{
			HUD->InitOverlay(PC, GetPlayerState());
		}
	}
}

void AEnergyCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	FirstPersonCameraComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Camera"));
}

void AEnergyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitialCharacter();
}

void AEnergyCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitialCharacter();
}

//////////////////////////////////////////////////////////////////////////// Input

void AEnergyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEnergyCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEnergyCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

float AEnergyCharacter::GetHealth_Implementation()
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		return EnergyPlayerState->GetHealth();
	}
	return 0.f;
}

float AEnergyCharacter::GetMaxHealth_Implementation()
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		return EnergyPlayerState->GetMaxHealth();
	}
	return 0.f;
}

float AEnergyCharacter::GetDamageBonus_Implementation()
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		return EnergyPlayerState->GetDamageBonus();
	}
	return 0.f;
}

float AEnergyCharacter::GetBaseDamageBonus_Implementation()
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		return EnergyPlayerState->GetBaseDamageBonus();
	}
	return 0.f;
}

float AEnergyCharacter::GetSpeed_Implementation()
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		return EnergyPlayerState->GetSpeed();
	}
	return 0.f;
}

void AEnergyCharacter::SetSpeed_Implementation(float NewSpeed)
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		EnergyPlayerState->SetSpeed(NewSpeed);
	}
}

void AEnergyCharacter::AddSpeed_Implementation(float InSpeed)
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		EnergyPlayerState->AddSpeed(InSpeed);
	}
}

void AEnergyCharacter::AddBaseSpeed_Implementation(float InBaseSpeed)
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		EnergyPlayerState->AddBaseSpeed(InBaseSpeed);
	}
}

void AEnergyCharacter::AddEnergyScore_Implementation(float InScore)
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		EnergyPlayerState->AddEnergyScore(InScore);
	}
}

void AEnergyCharacter::Die()
{
	bDeath = true;
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		EnergyPlayerState->SetEnergyScore(EnergyPlayerState->GetEnergyScore() * 0.8f);
		EnergyPlayerState->SetHealth(EnergyPlayerState->GetMaxHealth());
	}

	FTimerHandle ResTimerHandle;
	GetWorldTimerManager().SetTimer(ResTimerHandle, this, &AEnergyCharacter::ResStart, 3.f, false);
}

void AEnergyCharacter::ResStart()
{
	bDeath = false;
}

void AEnergyCharacter::OnSpeedupEffect_Implementation(float NewSpeed, float Duration)
{
	if (GetWorldTimerManager().IsTimerActive(SpeedupEffectTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(SpeedupEffectTimerHandle);
	}
	GetWorldTimerManager().SetTimer(SpeedupEffectTimerHandle, this, &AEnergyCharacter::ResetSpeed, Duration, false);
	Execute_SetSpeed(this, NewSpeed);
}

void AEnergyCharacter::OnDamageIncreaseEffect_Implementation(float NewDamageBonus, float Duration)
{
	if (GetWorldTimerManager().IsTimerActive(DamageBonusEffectTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(DamageBonusEffectTimerHandle);
	}
	GetWorldTimerManager().SetTimer(DamageBonusEffectTimerHandle, this, &AEnergyCharacter::ResetDamageBonus, Duration, false);
	Execute_SetDamageBonus(this, NewDamageBonus);
}

void AEnergyCharacter::ResetSpeed()
{
	Execute_SetSpeed(this, Execute_GetBaseSpeed(this));
}

void AEnergyCharacter::ResetDamageBonus()
{
	Execute_SetDamageBonus(this, Execute_GetBaseDamageBonus(this));
}

void AEnergyCharacter::SetBaseSpeed_Implementation(float NewBaseSpeed)
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		EnergyPlayerState->SetBaseSpeed(NewBaseSpeed);
	}
}

void AEnergyCharacter::SetEnergyScore_Implementation(float NewScore)
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		EnergyPlayerState->SetEnergyScore(NewScore);
	}
}

float AEnergyCharacter::GetBaseSpeed_Implementation()
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		return EnergyPlayerState->GetBaseSpeed();
	}
	return 0.f;
}

float AEnergyCharacter::GetEnergyScore_Implementation()
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		return EnergyPlayerState->GetEnergyScore();
	}
	return 0.f;
}

void AEnergyCharacter::SetHealth_Implementation(float NewHealth)
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		EnergyPlayerState->SetHealth(NewHealth);
	}
}

void AEnergyCharacter::SetMaxHealth_Implementation(float NewMaxHealth)
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		EnergyPlayerState->SetMaxHealth(NewMaxHealth);
	}
}

void AEnergyCharacter::SetDamageBonus_Implementation(float NewDamageBonus)
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		EnergyPlayerState->SetDamageBonus(NewDamageBonus);
	}
}

void AEnergyCharacter::AddDamageBonus_Implementation(float InDamageBonus)
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		EnergyPlayerState->AddDamageBonus(InDamageBonus);
	}
}

void AEnergyCharacter::AddBaseDamageBonus_Implementation(float InBaseDamageBonus)
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		EnergyPlayerState->AddBaseDamageBonus(InBaseDamageBonus);
	}
}

void AEnergyCharacter::SetBaseDamageBonus_Implementation(float NewBaseDamageBonus)
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		EnergyPlayerState->SetBaseDamageBonus(NewBaseDamageBonus);
	}
}

void AEnergyCharacter::AddHealth_Implementation(float InHealth)
{
	if (bDeath) return;
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		EnergyPlayerState->AddHealth(InHealth);
		if (EnergyPlayerState->GetHealth() <= 0)
		{
			Die();
		}
	}
}

void AEnergyCharacter::AddMaxHealth_Implementation(float InMaxHealth)
{
	if (AEnergyPlayerState* EnergyPlayerState = GetPlayerState<AEnergyPlayerState>())
	{
		EnergyPlayerState->AddMaxHealth(InMaxHealth);
	}
}

void AEnergyCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AEnergyCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}