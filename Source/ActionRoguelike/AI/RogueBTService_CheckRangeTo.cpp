// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBTService_CheckRangeTo.h"

#include "AIController.h"
#include "RogueGameTypes.h"
#include "BehaviorTree/BlackboardComponent.h"

URogueBTService_CheckRangeTo::URogueBTService_CheckRangeTo()
{
    TargetActorKey.SelectedKeyName = NAME_TargetActor;
}

void URogueBTService_CheckRangeTo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    check(BBComp);

    const AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
    if (TargetActor)
    {
        const AAIController* Controller = OwnerComp.GetAIOwner();

        const APawn* OwningPawn = Controller->GetPawn();
        check(OwningPawn);

        const float DistanceTo = FVector::Dist(TargetActor->GetActorLocation(), OwningPawn->GetActorLocation());

        const bool bWithinRange = DistanceTo < MaxAttackRange;

        const bool bHasLOS = Controller->LineOfSightTo(TargetActor);

        BBComp->SetValueAsBool(WithinRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
    }
}