// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "RoguePlayerCharacter.generated.h"

class URogueActionSystemComponent;
struct FInputActionInstance;
struct FInputActionValue;
struct FGameplayTag;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ACTIONROGUELIKE_API ARoguePlayerCharacter : public ACharacter, public IGenericTeamAgentInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ARoguePlayerCharacter();

    virtual FGenericTeamId GetGenericTeamId() const override;
    
protected:

    UPROPERTY(EditDefaultsOnly, Category = "Death")
    TObjectPtr<UAnimMontage> DeathMontage;
    
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> Input_Move;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> Input_Look;

    UPROPERTY(EditDefaultsOnly, Category="Input")
    TObjectPtr<UInputAction> Input_Jump;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> Input_PrimaryAttack;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> Input_SecondaryAttack;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> Input_SpecialAttack;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> Input_Sprint;
    
    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UCameraComponent> CameraComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<USpringArmComponent> SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<URogueActionSystemComponent> ActionSystemComponent;
    
    void Move(const FInputActionValue& InValue);

    void Look(const FInputActionInstance& InValue);

    void StartAction(FGameplayTag InActionName);
    
    void StopAction(FGameplayTag InActionName);
    
    void OnHealthChanged(FGameplayTag AttributeTag, float NewHealth, float OldHealth);
    
public:
    virtual void PostInitializeComponents() override;
    
    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
        AActor* DamageCauser) override;

    URogueActionSystemComponent* GetActionSystemComponent() const
    {
        return ActionSystemComponent;
    }
    
protected:
	
    FTimerHandle OverlayTimerHandle;    
};