// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileMagic.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


ARogueProjectileMagic::ARogueProjectileMagic()
{
    LoopedNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LoopedNiagaraComponent"));
    LoopedNiagaraComponent->SetupAttachment(SphereComponent);
    
    LoopedAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("LoopedAudioComponent"));
    LoopedAudioComponent->SetupAttachment(SphereComponent);
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
