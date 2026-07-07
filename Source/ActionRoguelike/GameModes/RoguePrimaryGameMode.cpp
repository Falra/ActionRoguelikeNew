// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePrimaryGameMode.h"

#include "RogueGameTypes.h"
#include "AI/RogueAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"


ARoguePrimaryGameMode::ARoguePrimaryGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickInterval = 1.0f;
}

void ARoguePrimaryGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (MonsterSpawnTable == nullptr)
    {
        return;
    }

    TArray<FMonsterSpawnData*> AllRows;
    MonsterSpawnTable->GetAllRows("SelectMonster", AllRows);

    const int32 SelectedIndex = FMath::RandRange(0, AllRows.Num() - 1);

    FMonsterSpawnData* SelectedRow = AllRows[SelectedIndex];

    const FQueryFinishedSignature CompleteDelegate = FQueryFinishedSignature::CreateUObject(this, &ThisClass::SpawnQueryCompleted,
        SelectedRow);

    FEnvQueryRequest Request(SpawnLocationQuery, this);
    Request.Execute(EEnvQueryRunMode::SingleResult, CompleteDelegate);
}

void ARoguePrimaryGameMode::SpawnQueryCompleted(TSharedPtr<FEnvQueryResult> QueryResult, FMonsterSpawnData* SelectedMonster)
{
    FVector SpawnLocation = QueryResult->GetItemAsLocation(0);

    SelectedMonster->MonsterClass.LoadAsync(
        FLoadSoftObjectPathAsyncDelegate::CreateUObject(this, &ThisClass::OnMonsterClassLoaded, SpawnLocation, SelectedMonster));
}

void ARoguePrimaryGameMode::OnMonsterClassLoaded(const FSoftObjectPath& LoadedObjectPath, UObject* LoadedObject, FVector SpawnLocation,
    FMonsterSpawnData* SelectedMonster)
{
    const FActorSpawnParameters SpawnParams;

    ARogueAICharacter* NewMonster = GetWorld()->SpawnActor<ARogueAICharacter>(SelectedMonster->MonsterClass.Get(), SpawnLocation,
        FRotator::ZeroRotator, SpawnParams);

    // Set attributes, add buffs/debuffs, etc.
}