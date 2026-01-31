// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EnergyBall.h"

#include "Character/EnergyCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnergyBall::AEnergyBall()
{
	PrimaryActorTick.bCanEverTick = false;
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	SetRootComponent(CollisionComp);

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnergyBall::OnOverlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionProfileName("NoCollision");
}

void AEnergyBall::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetWorldTimerManager().IsTimerActive(HiddenTimerHandle)) return;
	if (AEnergyCharacter* EnergyCharacter = Cast<AEnergyCharacter>(OtherActor))
	{
		EffectToCharacter(EnergyCharacter);
		AddScore(EnergyCharacter, 3);
		Hide();
		GetWorldTimerManager().SetTimer(HiddenTimerHandle, this, &AEnergyBall::Show, HiddenTime, false);
		ClientPickup();
	}
}


void AEnergyBall::BeginPlay()
{
	Super::BeginPlay();
}

void AEnergyBall::Hide()
{
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorHiddenInGame(true);
}

void AEnergyBall::Show()
{
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetActorHiddenInGame(false);
}

void AEnergyBall::AddScore_Implementation(AEnergyCharacter* EnergyCharacter, int32 Score)
{
	if (EnergyCharacter->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_AddEnergyScore(EnergyCharacter, Score);
	}
}

void AEnergyBall::ClientPickup_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
}


