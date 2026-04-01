// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionSystemComponent.h"

#include "RogueAction.h"
#include "RogueAttributeSet.h"


URogueActionSystemComponent::URogueActionSystemComponent()
{
    bWantsInitializeComponent = true;
    
    AttributeSetClass = URogueAttributeSet::StaticClass();
}

void URogueActionSystemComponent::InitializeComponent()
{
    Super::InitializeComponent();
    
    Attributes = NewObject<URogueAttributeSet>(this, AttributeSetClass);
    
    for (TFieldIterator<FStructProperty> PropIt(Attributes->GetClass()); PropIt; ++PropIt)
    {
        FRogueAttribute* FoundAttribute = PropIt->ContainerPtrToValuePtr<FRogueAttribute>(Attributes);
		
        FName AttributeTagName = FName("Attribute." + PropIt->GetName());
        FGameplayTag AttributeTag = FGameplayTag::RequestGameplayTag(AttributeTagName);

        CachedAttributes.Add(AttributeTag, FoundAttribute);
    }
    
    for (const auto ActionClass : DefaultActions)
    {
        if (ensure(ActionClass))
        {
            GrantAction(ActionClass);
        }
    }
}

void URogueActionSystemComponent::GrantAction(TSubclassOf<URogueAction> NewActionClass)
{
    URogueAction* NewAction = NewObject<URogueAction>(this, NewActionClass);
    Actions.Add(NewAction);
}

FRogueAttribute* URogueActionSystemComponent::GetAttribute(FGameplayTag InAttributeTag)
{
    FRogueAttribute** FoundAttribute = CachedAttributes.Find(InAttributeTag);

    return *FoundAttribute;
}

void URogueActionSystemComponent::ApplyHealthChange(float InValueChange)
{
    /*const float OldHealth = Attributes.Health;
    Attributes.Health = FMath::Clamp(Attributes.Health + InValueChange, 0.0f, Attributes.HealthMax);

    if (!FMath::IsNearlyEqual(OldHealth, Attributes.Health))
    {
        OnHealthChanged.Broadcast(Attributes.Health, OldHealth);
    }

    UE_LOG(LogTemp, Log, TEXT("New Health: %f, Max Health: %f"), Attributes.Health, Attributes.HealthMax);*/
}

bool URogueActionSystemComponent::IsFullHealth() const
{
    return true; //FMath::IsNearlyEqual(Attributes.HealthMax, Attributes.Health);
}

void URogueActionSystemComponent::StartAction(FGameplayTag InActionName)
{
    for (const auto& Action : Actions)
    {
        if (Action->GetActionName() == InActionName)
        {
            if (Action->CanStart())
            {
                Action->StartAction();
            }
            return;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("No Action found with name %s"), *InActionName.ToString());
}

void URogueActionSystemComponent::StopAction(FGameplayTag InActionName)
{
    for (const auto& Action : Actions)
    {
        if (Action->GetActionName() == InActionName && Action->IsRunning())
        {
            Action->StopAction();
            return;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("No Action found with name %s"), *InActionName.ToString());
}
