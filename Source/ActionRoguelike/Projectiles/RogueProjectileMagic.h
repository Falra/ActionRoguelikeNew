// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectile.h"
#include "RogueProjectileMagic.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARogueProjectileMagic : public ARogueProjectile
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ARogueProjectileMagic();

    virtual void PostInitializeComponents() override;
    
protected:
    
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    TSubclassOf<UDamageType> DmgTypeClass;
    
    UPROPERTY(EditDefaultsOnly, Category = "Components")
    TObjectPtr<UNiagaraComponent> LoopedNiagaraComponent;
    
    UPROPERTY(EditDefaultsOnly, Category = "Components")
    TObjectPtr<UAudioComponent> LoopedAudioComponent;
    
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TObjectPtr<UNiagaraSystem> ExplosionEffect;
    
    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    TObjectPtr<USoundBase> ExplosionSound;
    
    UFUNCTION()
    void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
        FVector NormalImpulse, const FHitResult& Hit);
};