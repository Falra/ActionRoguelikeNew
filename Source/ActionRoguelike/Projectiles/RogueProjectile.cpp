// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


ARogueProjectile::ARogueProjectile()
{
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    RootComponent = SphereComponent;
    SphereComponent->SetSphereRadius(16.0f);
    SphereComponent->SetCollisionProfileName("Projectile");
	
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoveComponent"));
    ProjectileMovementComponent->InitialSpeed = 2000.f;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

