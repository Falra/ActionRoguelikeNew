// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueInteractionComponent.h"

#include "RogueGameTypes.h"
#include "Core/RogueInteractionInterface.h"
#include "Engine/OverlapResult.h"

TAutoConsoleVariable<bool> CVarInteractionDebugDrawing(TEXT("game.interaction.DebugDraw"), false,
    TEXT("Enable interaction component debug rendering. (0 = off, 1 = enabled)"),
    ECVF_Cheat);


URogueInteractionComponent::URogueInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void URogueInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    const APlayerController* PC = CastChecked<APlayerController>(GetOwner());
    const FVector Center = PC->GetPawn()->GetActorLocation();
    
    constexpr ECollisionChannel CollisionChannel = COLLISION_INTERACTION;

    FCollisionShape Shape;
    Shape.SetSphere(InteractionRadius);

    TArray<FOverlapResult> Overlaps;
    GetWorld()->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, CollisionChannel, Shape);

    AActor* BestActor = nullptr;
    float HighestDotResult = -1.0;
    bool bEnabledDebugDraw = CVarInteractionDebugDrawing.GetValueOnGameThread();
    const FVector CameraDirection = PC->GetControlRotation().Vector();
        
    for (FOverlapResult& Overlap : Overlaps)
    {
        FVector OverlapLocation = Overlap.GetActor()->GetActorLocation();
        FVector OverlapDirection = (OverlapLocation - Center).GetSafeNormal();

        const float DotResult = FVector::DotProduct(OverlapDirection, CameraDirection);

        if (DotResult > HighestDotResult)
        {
            BestActor = Overlap.GetActor();
            HighestDotResult = DotResult;
        }
        
        if (bEnabledDebugDraw)
        {
            DrawDebugBox(GetWorld(), OverlapLocation, FVector(50.0f), FColor::Red);
            FString DebugString = FString::Printf(TEXT("Dot: %f"), DotResult);
            DrawDebugString(GetWorld(), OverlapLocation, DebugString, nullptr, FColor::White, 0.0f, true);
        }
    }

    SelectedActor = BestActor;
    if (bEnabledDebugDraw)
    {
        if (BestActor)
        {
            DrawDebugBox(GetWorld(), BestActor->GetActorLocation(), FVector(60.0f), FColor::Green);
        }
    
        DrawDebugSphere(GetWorld(), Center, InteractionRadius, 32, FColor::White);
    }
}

void URogueInteractionComponent::Interact() const
{
    if (SelectedActor && SelectedActor->Implements<URogueInteractionInterface>())
    {
        IRogueInteractionInterface::Execute_Interact(SelectedActor);
    }
}
