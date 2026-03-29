// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionSystemComponent.h"

#include "RogueAction.h"


URogueActionSystemComponent::URogueActionSystemComponent()
{
    bWantsInitializeComponent = true;
}

void URogueActionSystemComponent::InitializeComponent()
{
    Super::InitializeComponent();
    
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

void URogueActionSystemComponent::ApplyHealthChange(float InValueChange)
{
    const float OldHealth = Attributes.Health;
    Attributes.Health = FMath::Clamp(Attributes.Health + InValueChange, 0.0f, Attributes.HealthMax);

    if (!FMath::IsNearlyEqual(OldHealth, Attributes.Health))
    {
        OnHealthChanged.Broadcast(Attributes.Health, OldHealth);
    }

    UE_LOG(LogTemp, Log, TEXT("New Health: %f, Max Health: %f"), Attributes.Health, Attributes.HealthMax);
}

bool URogueActionSystemComponent::IsFullHealth() const
{
    return FMath::IsNearlyEqual(Attributes.HealthMax, Attributes.Health);
}

void URogueActionSystemComponent::StartAction(const FName& InActionName)
{
    for (const auto& Action : Actions)
    {
        if (Action->GetActionName() == InActionName)
        {
            Action->StartAction();
            return;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("No Action found with name %s"), *InActionName.ToString());
}
