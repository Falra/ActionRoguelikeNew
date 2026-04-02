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

UENUM(BlueprintType)
enum EAttributeModifyType
{
    Base,
    Modifier,
    OverrideBase,
    Invalid
};

// Native C++ delegate
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, FGameplayTag /*AttributeTag*/, float /*NewAttributeValue*/, float /*OldAttributeValue*/);
// Blueprint delegate
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnAttributeDynamicChanged, FGameplayTag, AttributeTag, float, NewAttributeValue, float, OldAttributeValue);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API URogueActionSystemComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URogueActionSystemComponent();

    virtual void BeginPlay() override;
    
    UFUNCTION(BlueprintCallable)
    void ApplyAttributeChange(UPARAM(meta = (Categories = "Attribute")) FGameplayTag AttributeTag, float Delta, EAttributeModifyType ModifyType);
    
    void StartAction(FGameplayTag InActionName);
    
    void StopAction(FGameplayTag InActionName);
    
    virtual void InitializeComponent() override;
    
    void GrantAction(TSubclassOf<URogueAction> NewActionClass);
    
    FRogueAttribute* GetAttribute(FGameplayTag InAttributeTag) const;

    UFUNCTION(BlueprintCallable)
    float GetAttributeValue(UPARAM(meta = (Categories = "Attribute")) FGameplayTag InAttributeTag) const;
    
    FOnAttributeChanged& GetAttributeListener(FGameplayTag AttributeTag);
    
    UFUNCTION(BlueprintCallable, DisplayName="Add Attribute Listener", meta = (Keywords = "events,delegate"))
    void AddDynamicAttributeListener(FOnAttributeDynamicChanged Event, UPARAM(meta = (Categories = "Attribute")) FGameplayTag AttributeTag);
    
    FGameplayTagContainer ActiveGameplayTags;
    
protected:

    UPROPERTY()
    TObjectPtr<URogueAttributeSet> Attributes;

    TMap<FGameplayTag, FRogueAttribute*> CachedAttributes;
    
    UPROPERTY(EditAnywhere, Category = "Attributes", NoClear)
    TSubclassOf<URogueAttributeSet> AttributeSetClass;

    TMap<FGameplayTag, FOnAttributeChanged> AttributeListeners;
    
    TMap<FGameplayTag, TArray<FOnAttributeDynamicChanged>> AttributeDynamicListeners;
    
    UPROPERTY(EditAnywhere, Category = "Actions")
    TArray<TSubclassOf<URogueAction>> DefaultActions;
    
    UPROPERTY()
    TArray<TObjectPtr<URogueAction>> Actions;
};