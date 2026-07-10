// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAICharacter.h"

#include "AIController.h"
#include "RogueGameInstance.h"
#include "SharedGameplayTags.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "ActionSystem/RogueAttributeSet.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void ARogueAICharacter::OnGameplayTagUpdated(FGameplayTag UpdatedTag, int32 NewCount)
{
    // Block/Allow movement
    if (UpdatedTag.MatchesTag(SharedGameplayTags::StatusEffect_Stunned))
    {
        const bool bWasAdded = NewCount > 0;

        const EMovementMode NewMoveMode = bWasAdded ? MOVE_None : MOVE_Walking;
        GetCharacterMovement()->SetMovementMode(NewMoveMode);

        // Pause ALL logic for Monster
        const AAIController* AIC = Cast<AAIController>(GetController());
        // Could be nullptr for dead monsters, in that case we shouldn't reach here anymore
        check(AIC);
        UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIC->GetBrainComponent());
        check(BTComp);

        // Alternatively, you could restart the logic to skip any 'waits' it might be in and get out of the stunned
        // picking a new thing to do right away (BTComp->StartLogic() BTComp->StopLogic())
        if (bWasAdded)
        {
            BTComp->PauseLogic("StunApplied");
        }
        else
        {
            BTComp->ResumeLogic("StunRemoved");
        }

        // Animation
        if (bWasAdded)
        {
            PlayAnimMontage(StunnedAnimation);
        }
    }
}

ARogueAICharacter::ARogueAICharacter()
{
    ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>(TEXT("ActionSystemComponent"));
    ActionSystemComponent->SetDefaultAttributeSet(URogueMonsterAttributeSet::StaticClass());
}

void ARogueAICharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    GetMesh()->SetOverlayMaterialMaxDrawDistance(1);

    ActionSystemComponent->GameplayTagUpdated.AddDynamic(this, &ThisClass::OnGameplayTagUpdated);
}

void ARogueAICharacter::BeginPlay()
{
    Super::BeginPlay();

    URogueGameInstance* GI = GetGameInstance<URogueGameInstance>();
    check(GI);
    GI->AliveMonsters.Add(this);
}

void ARogueAICharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    URogueGameInstance* GI = GetGameInstance<URogueGameInstance>();
    GI->AliveMonsters.RemoveSingleSwap(this, EAllowShrinking::No);

    Super::EndPlay(EndPlayReason);
}

float ARogueAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
    AActor* DamageCauser)
{
    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    ActionSystemComponent->ApplyAttributeChange(SharedGameplayTags::Attribute_Health, -ActualDamage, Base);

    GetMesh()->SetOverlayMaterialMaxDrawDistance(0);

    //GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
    GetMesh()->SetCustomPrimitiveDataFloat(0, GetWorld()->TimeSeconds);

    GetWorldTimerManager().SetTimer(OverlayTimerHandle, [this]()
    {
        GetMesh()->SetOverlayMaterialMaxDrawDistance(1);
    }, 1.0f, false);

    // @todo: handle death, remove from AliveMonsters array

    return ActualDamage;
}