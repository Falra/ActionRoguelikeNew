// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueEnvQueryContext_TargetActor.h"

#include "AIController.h"
#include "RogueGameTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void URogueEnvQueryContext_TargetActor::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    // Expected Character
    const APawn* QuerierPawn = Cast<APawn>(QueryInstance.Owner.Get());
    if (ensure(QuerierPawn))
    {
        AAIController* Controller = Cast<AAIController>(QuerierPawn->GetController());
        if (!IsValid(Controller))
        {
            UEnvQueryItemType_Actor::SetContextHelper(ContextData, QuerierPawn);
            return;
        }

        const UBlackboardComponent* BlackboardComponent = Controller->GetBlackboardComponent();
        if (!IsValid(BlackboardComponent))
        {
            UEnvQueryItemType_Actor::SetContextHelper(ContextData, QuerierPawn);
            return;
        }
        const AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(NAME_TargetActor));
        UEnvQueryItemType_Actor::SetContextHelper(ContextData, TargetActor);
    }
}