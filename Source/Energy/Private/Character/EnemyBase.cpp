// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyBase.h"

#include "AIController.h"
#include "AI/EnergyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/EnergyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/EnergyUserWidget.h"

AEnemyBase::AEnemyBase()
{
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());

	Health = MaxHealth;
}

void AEnemyBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEnemyBase, Health);
	DOREPLIFETIME(AEnemyBase, MaxHealth);
	DOREPLIFETIME(AEnemyBase, Damage);
}

void AEnemyBase::Die()
{
	Super::Die();
	SetLifeSpan(LifeSpan);
	if (AIController) AIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

void AEnemyBase::MultiClientPlayAttackMontage_Implementation()
{
	PlayAnimMontage(AttackMontage);
}

void AEnemyBase::MulticastHandleDeath_Implementation()
{
	if (IsValid(DeathSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	bDead = true;
}

void AEnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (!HasAuthority()) return;
	AIController = Cast<AEnergyAIController>(NewController);
	if (BehaviorTree && AIController)
	{
		AIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		AIController->RunBehaviorTree(BehaviorTree);
	}
}

void AEnemyBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		// 创建材质动态实例
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeLine(DynamicMatInst);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeLine(DynamicMatInst);
	}
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	if (UEnergyUserWidget* AuraUserWidget = Cast<UEnergyUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}
	OnHealthChanged.Broadcast(Health);
	OnMaxHealthChanged.Broadcast(MaxHealth);
}

void AEnemyBase::AddHealth_Implementation(float InHealth)
{
	Health += InHealth;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
	if (Health <= 0)
	{
		Die();
	}
}

void AEnemyBase::AddMaxHealth_Implementation(float InMaxHealth)
{
	MaxHealth += InMaxHealth;
	OnMaxHealthChanged.Broadcast(MaxHealth);
}


void AEnemyBase::SetHealth_Implementation(float NewHealth)
{
	Health = NewHealth;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}

void AEnemyBase::SetMaxHealth_Implementation(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
	OnMaxHealthChanged.Broadcast(MaxHealth);
}

void AEnemyBase::GetLivePlayersWithRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
		const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius), SphereParams);

		for (FOverlapResult& Overlap : Overlaps)
		{
			if (AEnergyCharacter* EnergyCharacter = Cast<AEnergyCharacter>(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(Overlap.GetActor());
			}
		}
	}
}


void AEnemyBase::OnRep_Health(float OldHealth)
{
	OnHealthChanged.Broadcast(Health);
}

void AEnemyBase::OnRep_MaxHealth(float OldMaxHealth)
{
	OnMaxHealthChanged.Broadcast(MaxHealth);
}

void AEnemyBase::OnRep_Damage(float OldDamage)
{
}

