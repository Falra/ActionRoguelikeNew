// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCharacter.h"

#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Projectiles/RogueProjectileMagic.h"


// Sets default values
ARogueCharacter::ARogueCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->bUsePawnControlRotation = true;
    
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent);
    
    MuzzleSocketName = "Muzzle_01";
}

// Called when the game starts or when spawned
void ARogueCharacter::BeginPlay()
{
    Super::BeginPlay();
    
}

void ARogueCharacter::Move(const FInputActionValue& InValue)
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

void ARogueCharacter::Look(const FInputActionInstance& InValue)
{
    const FVector2D InputValue = InValue.GetValue().Get<FVector2D>();
	
    AddControllerPitchInput(InputValue.Y);
    AddControllerYawInput(InputValue.X);
}

void ARogueCharacter::PrimaryAttack()
{
    PlayAnimMontage(AttackMontage);
    
    FTimerHandle AttackTimerHandle;
    constexpr float AttackDelayTime = 0.2f;
    GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ThisClass::AttackTimerElapsed, AttackDelayTime);
}

void ARogueCharacter::AttackTimerElapsed()
{
    const FVector SpawnLocation = GetMesh()->GetSocketLocation(MuzzleSocketName);
    const FRotator SpawnRotation = GetControlRotation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Instigator = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
}

// Called every frame
void ARogueCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARogueCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (!EnhancedInput)
    {
        return;
    }
    
    EnhancedInput->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ARogueCharacter::Move);
    EnhancedInput->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ARogueCharacter::Look);
    
    EnhancedInput->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ARogueCharacter::PrimaryAttack);
}

