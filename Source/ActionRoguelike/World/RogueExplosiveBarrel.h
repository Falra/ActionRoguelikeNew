// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueExplosiveBarrel.generated.h"

class UNiagaraComponent;
class URadialForceComponent;
class UNiagaraSystem;

UCLASS()
class ACTIONROGUELIKE_API ARogueExplosiveBarrel : public AActor
{
    GENERATED_BODY()

public:
    ARogueExplosiveBarrel();

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
        AActor* DamageCauser) override;

protected:
    
    UPROPERTY(EditAnywhere, Category = "Explosive Barrel")
    float ExplosionDelay = 3.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Explosive Barrel")
    TObjectPtr<UNiagaraSystem> ExplosionEffect;
	
    UPROPERTY(EditDefaultsOnly, Category = "Explosive Barrel")
    TObjectPtr<USoundBase> ExplosionSound;
	
    UPROPERTY(EditDefaultsOnly, Category = "Explosive Barrel")
    TObjectPtr<UNiagaraSystem> BurningEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Explosive Barrel")
    TObjectPtr<USoundBase> BurningSound;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UStaticMeshComponent> MeshComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<URadialForceComponent> RadialForceComponent;

    UFUNCTION(BlueprintCallable, Category = "Explosive Barrel")
    void Explode();

    bool bExploded = false;

    FTimerHandle ExplosionTimerHandle;
	
    UPROPERTY()
    TObjectPtr<UNiagaraComponent> ActiveBurningEffectComp = nullptr;
	
    UPROPERTY()
    TObjectPtr<UAudioComponent> ActiveBurningSoundComp = nullptr;

};