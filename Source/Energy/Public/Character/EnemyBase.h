// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/EnergyCharacterBase.h"
#include "EnemyBase.generated.h"

class AEnergyAIController;
class AEnergyCharacter;
class AAIController;
class UNiagaraSystem;
class UBehaviorTree;
class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
/**
 * 
 */
UCLASS()
class ENERGY_API AEnemyBase : public AEnergyCharacterBase
{
	GENERATED_BODY()
public:
	AEnemyBase();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	virtual void Die() override;

	void SetCombatTarget(AActor* InCombatTarget) { CombatTarget = InCombatTarget; }
	AActor* GetCombatTarget() { return CombatTarget;}
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetAttackMontage() { return AttackMontage;}
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void MultiClientPlayAttackMontage();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHandleDeath();

	/**ICombatInterface*/
	FORCEINLINE virtual float GetHealth_Implementation() override { return Health; }
	FORCEINLINE virtual float GetMaxHealth_Implementation() override { return MaxHealth; }
	FORCEINLINE virtual float GetDamage_Implementation() override { return Damage; }

	virtual void AddHealth_Implementation(float InHealth) override;
	virtual void AddMaxHealth_Implementation(float InMaxHealth) override;

	virtual void SetHealth_Implementation(float NewHealth) override;
	virtual void SetMaxHealth_Implementation(float NewMaxHealth) override;
	/**ICombatInterface*/

	UFUNCTION(BlueprintCallable)
	void GetLivePlayersWithRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
		const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	UPROPERTY(EditAnywhere, Category="Combat")
	FName WeaponTipSocketName = FName("TipSocket");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(VisibleAnywhere, Category="AI")
	TObjectPtr<AEnergyAIController> AIController;
	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY(BlueprintReadWrite, Category="Combat")
	TObjectPtr<AActor> CombatTarget;
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UAnimMontage> AttackMontage;

	virtual void PossessedBy(AController* NewController) override;
	bool bDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dissolve")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dissolve")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	void Dissolve();
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeLine(UMaterialInstanceDynamic* DynamicMaterialInstance);
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeLine(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	UNiagaraSystem* BloodEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float BaseWalkSpeed = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float LifeSpan = 5.f;
private:
	UPROPERTY(ReplicatedUsing=OnRep_Health)
	float Health = 100.f;
	UPROPERTY(ReplicatedUsing=OnRep_MaxHealth)
	float MaxHealth = 50.f;
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing=OnRep_Damage)
	float Damage = 10.f;

	UFUNCTION()
	void OnRep_Health(float NewHealth);
	UFUNCTION()
	void OnRep_MaxHealth(float NewMaxHealth);
	UFUNCTION()
	void OnRep_Damage(float NewDamage);
};
