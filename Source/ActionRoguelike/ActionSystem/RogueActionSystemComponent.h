// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "RogueActionSystemComponent.generated.h"

class URogueAttributeSet;
class URogueAction;
struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, OldHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API URogueActionSystemComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URogueActionSystemComponent();

    void ApplyHealthChange(float InValueChange);

    bool IsFullHealth() const;

    float GetHealth() const { return 0.0f; } //Attributes.Health; }

    float GetHealthMax() const { return 0.0f; } // Attributes.HealthMax; }
    
    void StartAction(FGameplayTag InActionName);
    
    void StopAction(FGameplayTag InActionName);
    
    virtual void InitializeComponent() override;
    
    void GrantAction(TSubclassOf<URogueAction> NewActionClass);
    
    FGameplayTagContainer ActiveGameplayTags;
    
    UPROPERTY(BlueprintAssignable)
    FOnHealthChanged OnHealthChanged;
    
protected:

    UPROPERTY()
    TObjectPtr<URogueAttributeSet> Attributes;

    UPROPERTY(EditAnywhere, Category = "Attributes", NoClear)
    TSubclassOf<URogueAttributeSet> AttributeSetClass;

    UPROPERTY(EditAnywhere, Category = "Actions")
    TArray<TSubclassOf<URogueAction>> DefaultActions;
    
    UPROPERTY()
    TArray<TObjectPtr<URogueAction>> Actions;
};