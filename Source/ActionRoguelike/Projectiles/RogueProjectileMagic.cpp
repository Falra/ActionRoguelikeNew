// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileMagic.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ARogueProjectileMagic::ARogueProjectileMagic()
{
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    RootComponent = SphereComponent;
    SphereComponent->SetSphereRadius(16.0f);
    SphereComponent->SetCollisionProfileName("Projectile");
    
    LoopedNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LoopedNiagaraComponent"));
    LoopedNiagaraComponent->SetupAttachment(SphereComponent);
    
    LoopedAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("LoopedAudioComponent"));
    LoopedAudioComponent->SetupAttachment(SphereComponent);
    
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->InitialSpeed = 2000.0f;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void ARogueProjectileMagic::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    
    SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
    
    SphereComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnActorHit);
}

void ARogueProjectileMagic::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    FVector NormalImpulse, const FHitResult& Hit)
{
    const FVector HitFromDirection = GetActorRotation().Vector();

    UGameplayStatics::ApplyPointDamage(OtherActor, 10.f, HitFromDirection, Hit, GetInstigatorController(), this, DmgTypeClass);

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation());

    UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), FRotator::ZeroRotator);

    Destroy();    
}
