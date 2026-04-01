// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "RogueActionSystemComponent.generated.h"

struct FRogueAttribute;
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
    
    void StartAction(FGameplayTag InActionName);
    
    void StopAction(FGameplayTag InActionName);
    
    virtual void InitializeComponent() override;
    
    void GrantAction(TSubclassOf<URogueAction> NewActionClass);
    
    FRogueAttribute* GetAttribute(FGameplayTag InAttributeTag);
    
    FGameplayTagContainer ActiveGameplayTags;
    
    UPROPERTY(BlueprintAssignable)
    FOnHealthChanged OnHealthChanged;
    
protected:

    UPROPERTY()
    TObjectPtr<URogueAttributeSet> Attributes;

    TMap<FGameplayTag, FRogueAttribute*> CachedAttributes;
    
    UPROPERTY(EditAnywhere, Category = "Attributes", NoClear)
    TSubclassOf<URogueAttributeSet> AttributeSetClass;

    UPROPERTY(EditAnywhere, Category = "Actions")
    TArray<TSubclassOf<URogueAction>> DefaultActions;
    
    UPROPERTY()
    TArray<TObjectPtr<URogueAction>> Actions;
};