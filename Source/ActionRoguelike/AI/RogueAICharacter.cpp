// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAICharacter.h"

#include "SharedGameplayTags.h"
#include "ActionSystem/RogueActionSystemComponent.h"


ARogueAICharacter::ARogueAICharacter()
{
    ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>(TEXT("ActionSystemComponent"));
}

float ARogueAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
    AActor* DamageCauser)
{
    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    ActionSystemComponent->ApplyAttributeChange(SharedGameplayTags::Attribute_Health, -ActualDamage, Base);

    return ActualDamage;
}