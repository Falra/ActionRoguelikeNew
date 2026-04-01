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

UENUM()
enum EAttributeModifyType
{
    Base,
    Modifier,
    OverrideBase,
    Invalid
};

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, FGameplayTag /*AttributeTag*/, float /*NewAttributeValue*/, float /*OldAttributeValue*/);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API URogueActionSystemComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URogueActionSystemComponent();

    void ApplyAttributeChange(FGameplayTag AttributeTag, float Delta, EAttributeModifyType ModifyType);
    
    void StartAction(FGameplayTag InActionName);
    
    void StopAction(FGameplayTag InActionName);
    
    virtual void InitializeComponent() override;
    
    void GrantAction(TSubclassOf<URogueAction> NewActionClass);
    
    FRogueAttribute* GetAttribute(FGameplayTag InAttributeTag);
    
    FOnAttributeChanged& GetAttributeListener(FGameplayTag AttributeTag);
    
    FGameplayTagContainer ActiveGameplayTags;
    
protected:

    UPROPERTY()
    TObjectPtr<URogueAttributeSet> Attributes;

    TMap<FGameplayTag, FRogueAttribute*> CachedAttributes;
    
    UPROPERTY(EditAnywhere, Category = "Attributes", NoClear)
    TSubclassOf<URogueAttributeSet> AttributeSetClass;

    TMap<FGameplayTag, FOnAttributeChanged> AttributeListeners;
    
    UPROPERTY(EditAnywhere, Category = "Actions")
    TArray<TSubclassOf<URogueAction>> DefaultActions;
    
    UPROPERTY()
    TArray<TObjectPtr<URogueAction>> Actions;
};