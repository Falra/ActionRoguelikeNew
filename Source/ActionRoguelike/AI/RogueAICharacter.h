// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RogueAICharacter.generated.h"

struct FGameplayTag;
class URogueActionSystemComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARogueAICharacter : public ACharacter
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<URogueActionSystemComponent> ActionSystemComponent;

    UFUNCTION()
    void OnGameplayTagUpdated(FGameplayTag UpdatedTag, int32 NewCount);

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TObjectPtr<UAnimMontage> StunnedAnimation;

public:
    ARogueAICharacter();

    virtual void PostInitializeComponents() override;


    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
        AActor* DamageCauser) override;

protected:
    FTimerHandle OverlayTimerHandle;
};