// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCoinPickupSubsystem.h"

void URogueCoinPickupSubsystem::AddCoinPickups(const TArray<FVector>& NewLocations, const TArray<int32>& NewAmounts)
{
    CoinLocations.Append(NewLocations);
    CoinAmounts.Append(NewAmounts);
}

void URogueCoinPickupSubsystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    const UWorld* World = GetWorld();
    for (int i = 0; i < CoinLocations.Num(); ++i)
    {
        DrawDebugPoint(World, CoinLocations[i], 8.0f, FColor::White);
    }
}