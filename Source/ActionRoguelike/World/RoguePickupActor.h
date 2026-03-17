// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoguePickupActor.generated.h"

class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API ARoguePickupActor : public AActor
{
    GENERATED_BODY()

public:
    ARoguePickupActor();

    virtual void PostInitializeComponents() override;

protected:
    
    UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<USphereComponent> OverlapComponent;
	
    UFUNCTION()
    virtual void OnActorOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};