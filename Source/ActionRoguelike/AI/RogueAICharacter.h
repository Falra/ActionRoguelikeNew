// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RogueAICharacter.generated.h"

class URogueActionSystemComponent;

UCLASS()
class ACTIONROGUELIKE_API ARogueAICharacter : public ACharacter
{
    GENERATED_BODY()

protected:
	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<URogueActionSystemComponent> ActionSystemComponent;
    
public:

    ARogueAICharacter();
    
    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	
    FTimerHandle OverlayTimerHandle;
};