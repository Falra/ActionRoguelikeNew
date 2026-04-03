// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "SharedGameplayTags.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "ActionSystem/RogueAttributeSet.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ARoguePlayerCharacter::ARoguePlayerCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->bUsePawnControlRotation = true;
    
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent);
    
    ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>(TEXT("ActionSystemComp"));
    ActionSystemComponent->SetDefaultAttributeSet(URoguePlayerAttributeSet::StaticClass());
}

void ARoguePlayerCharacter::Move(const FInputActionValue& InValue)
{
    const FVector2D InputValue = InValue.Get<FVector2D>();

    FRotator ControlRot = GetControlRotation();
    ControlRot.Pitch = 0.0f;

    // Forward/Back
    AddMovementInput(ControlRot.Vector(), InputValue.X);
    
    // Sideways
    const FVector RightDirection = ControlRot.RotateVector(FVector::RightVector);
    AddMovementInput(RightDirection, InputValue.Y);
}

void ARoguePlayerCharacter::Look(const FInputActionInstance& InValue)
{
    const FVector2D InputValue = InValue.GetValue().Get<FVector2D>();
	
    AddControllerPitchInput(InputValue.Y);
    AddControllerYawInput(InputValue.X);
}

void ARoguePlayerCharacter::StartAction(FGameplayTag InActionName)
{
    ActionSystemComponent->StartAction(InActionName);
}

void ARoguePlayerCharacter::StopAction(FGameplayTag InActionName)
{
    ActionSystemComponent->StopAction(InActionName);
}

void ARoguePlayerCharacter::OnHealthChanged(FGameplayTag AttributeTag, float NewHealth, float OldHealth)
{
    const float DamageAmount = NewHealth - OldHealth;
    // Died?
    if (FMath::IsNearlyZero(NewHealth) && DamageAmount > 0.0f)
    {
        DisableInput(nullptr);

        GetMovementComponent()->StopMovementImmediately();

        PlayAnimMontage(DeathMontage);
    }
}

void ARoguePlayerCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    FOnAttributeChanged& Event = ActionSystemComponent->GetAttributeListener(SharedGameplayTags::Attribute_Health);
    Event.AddUObject(this, &ThisClass::OnHealthChanged);
}

// Called to bind functionality to input
void ARoguePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (!EnhancedInput)
    {
        return;
    }
    
    EnhancedInput->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Move);
    EnhancedInput->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Look);
    
    EnhancedInput->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Jump);
    
    EnhancedInput->BindAction(Input_Sprint, ETriggerEvent::Started, this, &ThisClass::StartAction, SharedGameplayTags::Action_Sprint.GetTag());
    EnhancedInput->BindAction(Input_Sprint, ETriggerEvent::Completed, this, &ThisClass::StopAction, SharedGameplayTags::Action_Sprint.GetTag());
    
    // Projectile Attacks
    EnhancedInput->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ThisClass::StartAction, SharedGameplayTags::Action_PrimaryAttack.GetTag());
    EnhancedInput->BindAction(Input_SecondaryAttack, ETriggerEvent::Triggered, this, &ThisClass::StartAction, SharedGameplayTags::Action_SecondaryAttack.GetTag());
    EnhancedInput->BindAction(Input_SpecialAttack, ETriggerEvent::Triggered, this, &ThisClass::StartAction, SharedGameplayTags::Action_SpecialAttack.GetTag());
}

float ARoguePlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
    AActor* DamageCauser)
{
    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    ActionSystemComponent->ApplyAttributeChange(SharedGameplayTags::Attribute_Health, -ActualDamage, Base);

    // Damage to Rage ratio
    // (We could expose this as a global/general ratio or as an Attribute that can be improved through gameplay)
    const float RageToAdd = DamageAmount * 0.75f;
    ActionSystemComponent->ApplyAttributeChange(SharedGameplayTags::Attribute_Rage, RageToAdd, Modifier);
    
    return ActualDamage;
}

