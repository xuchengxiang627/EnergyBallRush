// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnergyCharacterBase.h"
#include "GameFramework/Character.h"
#include "Interface/ActorEffectInterface.h"
#include "Logging/LogMacros.h"
#include "EnergyCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AEnergyCharacter : public AEnergyCharacterBase, public IActorEffectInterface
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	// USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	virtual void InitialCharacter() override;

	FTimerHandle SpeedupEffectTimerHandle;
	FTimerHandle DamageBonusEffectTimerHandle;
protected:
	virtual void BeginPlay();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface
public:
	AEnergyCharacter();
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Returns Mesh1P subobject **/
	// USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/** ICombatInterface */
	virtual float GetHealth_Implementation() override;
	virtual float GetMaxHealth_Implementation() override;
	virtual float GetDamageBonus_Implementation() override;
	virtual float GetBaseDamageBonus_Implementation() override;
	virtual float GetSpeed_Implementation() override;
	virtual float GetBaseSpeed_Implementation() override;
	virtual float GetEnergyScore_Implementation() override;

	virtual void SetHealth_Implementation(float NewHealth) override;
	virtual void SetMaxHealth_Implementation(float NewMaxHealth) override;
	virtual void SetDamageBonus_Implementation(float NewDamageBonus) override;
	virtual void SetBaseDamageBonus_Implementation(float NewBaseDamageBonus) override;
	virtual void SetSpeed_Implementation(float NewSpeed) override;
	virtual void SetBaseSpeed_Implementation(float NewBaseSpeed) override;
	virtual void SetEnergyScore_Implementation(float NewScore) override;

	virtual void AddHealth_Implementation(float InHealth) override;
	virtual void AddMaxHealth_Implementation(float InMaxHealth) override;
	virtual void AddDamageBonus_Implementation(float InDamageBonus) override;
	virtual void AddBaseDamageBonus_Implementation(float InBaseDamageBonus) override;
	virtual void AddSpeed_Implementation(float InSpeed) override;
	virtual void AddBaseSpeed_Implementation(float InBaseSpeed) override;
	virtual void AddEnergyScore_Implementation(float InScore) override;

	virtual void Die() override;
	/** ICombatInterface */

	/** IActorEffectInterface */
	virtual void OnSpeedupEffect_Implementation(float NewSpeed, float Duration) override;
	virtual void OnDamageIncreaseEffect_Implementation(float NewDamageBonus, float Duration) override;
	/** IActorEffectInterface */
	void ResetSpeed();
	void ResetDamageBonus();
};



