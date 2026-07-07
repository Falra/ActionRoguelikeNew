// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueEnvQueryContext_AlivePlayers.h"

#include "EngineUtils.h"
#include "Core/RogueGameplayStatics.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Player/RoguePlayerCharacter.h"

void URogueEnvQueryContext_AlivePlayers::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    Super::ProvideContext(QueryInstance, ContextData);

    TArray<AActor*> AlivePlayers;

    for (ARoguePlayerCharacter* PlayerCharacter : TActorRange<ARoguePlayerCharacter>(QueryInstance.World))
    {
        if (URogueGameplayStatics::IsAlive(PlayerCharacter))
        {
            AlivePlayers.Add(PlayerCharacter);
        }
    }

    UEnvQueryItemType_Actor::SetContextHelper(ContextData, AlivePlayers);
}