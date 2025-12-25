// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueInteractionComponent.h"

#include "Engine/OverlapResult.h"


URogueInteractionComponent::URogueInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void URogueInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    const APlayerController* PC = CastChecked<APlayerController>(GetOwner());
    const FVector Center = PC->GetPawn()->GetActorLocation();
    
    constexpr ECollisionChannel CollisionChannel = ECC_Visibility;

    FCollisionShape Shape;
    Shape.SetSphere(InteractionRadius);

    TArray<FOverlapResult> Overlaps;
    GetWorld()->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, CollisionChannel, Shape);

    DrawDebugSphere(GetWorld(), Center, InteractionRadius, 32, FColor::White);

    AActor* BestActor = nullptr;
    float HighestDotResult = -1.0;
    const FVector CameraDirection = PC->GetControlRotation().Vector();
        
    for (FOverlapResult& Overlap : Overlaps)
    {
        FVector OverlapLocation = Overlap.GetActor()->GetActorLocation();
		
        DrawDebugBox(GetWorld(), OverlapLocation, FVector(50.0f), FColor::Red);

        FVector OverlapDirection = (OverlapLocation - Center).GetSafeNormal();

        const float DotResult = FVector::DotProduct(OverlapDirection, CameraDirection);

        FString DebugString = FString::Printf(TEXT("Dot: %f"), DotResult);

        DrawDebugString(GetWorld(), OverlapLocation, DebugString, nullptr, FColor::White, 0.0f, true);

        if (DotResult > HighestDotResult)
        {
            BestActor = Overlap.GetActor();
            HighestDotResult = DotResult;
        }
    }

    if (BestActor)
    {
        DrawDebugBox(GetWorld(), BestActor->GetActorLocation(), FVector(60.0f), FColor::Green);
    }
}

