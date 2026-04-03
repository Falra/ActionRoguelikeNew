// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAttributeSet.h"

#include "RogueActionSystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

URogueHealthAttributeSet::URogueHealthAttributeSet()
{
    Health = FRogueAttribute(100);
    HealthMax = FRogueAttribute(Health.GetValue());
}

URoguePawnAttributeSet::URoguePawnAttributeSet()
{
    MoveSpeed = FRogueAttribute(550);
    MoveSpeedMultiplier = FRogueAttribute(1.0f);
}

URogueMonsterAttributeSet::URogueMonsterAttributeSet()
{
    MoveSpeed = FRogueAttribute(450);
}

void URogueHealthAttributeSet::PostAttributeChanged()
{
    Super::PostAttributeChanged();
    
    Health.Base = FMath::Clamp(Health.Base, 0.0f, HealthMax.GetValue());
}


void URoguePawnAttributeSet::PostAttributeChanged()
{
    Super::PostAttributeChanged();

    ApplyMoveSpeed();
}

void URoguePawnAttributeSet::InitializeAttributes()
{
    Super::InitializeAttributes();

    ApplyMoveSpeed();
}

void URoguePawnAttributeSet::ApplyMoveSpeed()
{
    const ACharacter* OwningCharacter = Cast<ACharacter>(GetOwningComponent()->GetOwner());
    OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed.GetValue() * MoveSpeedMultiplier.GetValue();
}

URogueActionSystemComponent* URogueAttributeSet::GetOwningComponent() const
{
    return Cast<URogueActionSystemComponent>(GetOuter());
}