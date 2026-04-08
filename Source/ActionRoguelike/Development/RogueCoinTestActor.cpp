// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCoinTestActor.h"

#include "NavigationSystem.h"
#include "Pickups/RogueCoinPickupSubsystem.h"


ARogueCoinTestActor::ARogueCoinTestActor()
{
    DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
    RootComponent = DefaultSceneComponent;

}

void ARogueCoinTestActor::SpawnCoins(int32 SpawnCount)
{
    TArray<FVector> CoinLocations;
    CoinLocations.Reserve(SpawnCount);
    TArray<int32> CoinAmounts;
    CoinAmounts.Reserve(SpawnCount);

    const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(this);
    const FVector ActorLocation = GetActorLocation();

    for (int i = 0; i < SpawnCount; ++i)
    {
        FNavLocation NavLocation;
        NavSystem->GetRandomPointInNavigableRadius(ActorLocation, 1024, NavLocation);

        CoinLocations.Add(NavLocation.Location);
        CoinAmounts.Add(10);
    }

    URogueCoinPickupSubsystem* CoinSystem = GetWorld()->GetSubsystem<URogueCoinPickupSubsystem>();
    CoinSystem->AddCoinPickups(CoinLocations, CoinAmounts);
}


