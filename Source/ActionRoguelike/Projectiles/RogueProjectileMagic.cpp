// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileMagic.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
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
    
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->InitialSpeed = 2000.0f;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void ARogueProjectileMagic::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    
    SphereComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnActorHit);
}

void ARogueProjectileMagic::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    FVector NormalImpulse, const FHitResult& Hit)
{
    // @todo: create our own damage type
    const TSubclassOf<UDamageType> DmgTypeClass = UDamageType::StaticClass();
	
    UGameplayStatics::ApplyDamage(OtherActor, 10.f, GetInstigatorController(), this, DmgTypeClass);

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation());

    Destroy();    
}
