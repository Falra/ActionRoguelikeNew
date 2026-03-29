// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RogueActionSystemComponent.generated.h"

class URogueAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, OldHealth);

USTRUCT(BlueprintType)
struct FRogueAttributeSet
{
    GENERATED_BODY()
    
    FRogueAttributeSet()
        : Health(100.0f), HealthMax(100.0f) {}
	
    UPROPERTY(BlueprintReadOnly)
    float Health;
    
    UPROPERTY(BlueprintReadOnly)
    float HealthMax;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API URogueActionSystemComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URogueActionSystemComponent();

    void ApplyHealthChange(float InValueChange);

    bool IsFullHealth() const;

    float GetHealth() const { return Attributes.Health; }

    float GetHealthMax() const { return Attributes.HealthMax; }
    
    void StartAction(const FName& InActionName);
    
    virtual void InitializeComponent() override;
    
    UPROPERTY(BlueprintAssignable)
    FOnHealthChanged OnHealthChanged;
    
protected:

    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FRogueAttributeSet Attributes;
    
    UPROPERTY()
    TArray<TObjectPtr<URogueAction>> Actions;
};