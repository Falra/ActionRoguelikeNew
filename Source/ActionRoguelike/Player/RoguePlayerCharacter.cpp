// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ActionRoguelike/RogueGameTypes.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/RogueProjectileMagic.h"


TAutoConsoleVariable<float> CVarProjectileAdjustmentDebugDrawing(TEXT("game.projectile.DebugDraw"), 0.0f,
    TEXT("Enable projectile aim adjustment debug rendering. (0 = off, > 0 is duration)"),
    ECVF_Cheat);

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
    
    MuzzleSocketName = "Muzzle_01";
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

void ARoguePlayerCharacter::StartProjectileAttack(TSubclassOf<ARogueProjectile> ProjectileClass)
{
    PlayAnimMontage(AttackMontage);
    
    UNiagaraFunctionLibrary::SpawnSystemAttached(CastingEffect, GetMesh(), MuzzleSocketName,
        FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);

    UGameplayStatics::PlaySound2D(this, CastingSound);
    
    FTimerHandle AttackTimerHandle;
    constexpr float AttackDelayTime = 0.2f;
    // Passing in the projectile as the parameter
    FTimerDelegate Delegate;
    Delegate.BindUObject(this, &ARoguePlayerCharacter::AttackTimerElapsed, ProjectileClass);
    GetWorldTimerManager().SetTimer(AttackTimerHandle, Delegate, AttackDelayTime, false);
}

void ARoguePlayerCharacter::AttackTimerElapsed(TSubclassOf<ARogueProjectile> ProjectileClass)
{
    const FVector SpawnLocation = GetMesh()->GetSocketLocation(MuzzleSocketName);
    const FVector EyeLocation = CameraComponent->GetComponentLocation();
    const FRotator EyeRotation = GetControlRotation();

    const FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 5000.0f);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    UWorld* World = GetWorld();

    FVector AdjustTargetLocation;
    FHitResult Hit;
    if (World->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_PROJECTILE, QueryParams))
    {
        AdjustTargetLocation = Hit.Location;
    }
    else
    {
        AdjustTargetLocation = TraceEnd;
    }

    const FRotator SpawnRotation = (AdjustTargetLocation - SpawnLocation).Rotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Instigator = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    AActor* NewProjectile = World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

    MoveIgnoreActorAdd(NewProjectile);
    
#if !UE_BUILD_SHIPPING
    const float DebugDrawDuration = CVarProjectileAdjustmentDebugDrawing.GetValueOnGameThread();
    if (DebugDrawDuration > 0.0f)
    {
        // the hit location or trace end
        DrawDebugBox(World, AdjustTargetLocation, FVector(20.0f), FColor::Green, false, DebugDrawDuration);

        // adjustment line trace
        DrawDebugLine(World, EyeLocation, TraceEnd, FColor::Green, false, DebugDrawDuration);

        // New projectile path
        DrawDebugLine(World, SpawnLocation, AdjustTargetLocation, FColor::Yellow, false, DebugDrawDuration);

        // the original path of the projectile
        DrawDebugLine(World, SpawnLocation, SpawnLocation + (GetControlRotation().Vector() * 5000.0f), FColor::Purple,
            false, DebugDrawDuration);
    }
#endif
}

void ARoguePlayerCharacter::OnHealthChanged(float NewHealth, float OldHealth)
{
    // Died?
    if (FMath::IsNearlyZero(NewHealth))
    {
        DisableInput(nullptr);

        GetMovementComponent()->StopMovementImmediately();

        PlayAnimMontage(DeathMontage);
    }
}

void ARoguePlayerCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    ActionSystemComponent->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged);
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
    
    // Projectile Attacks
    EnhancedInput->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this,
        &ThisClass::StartProjectileAttack, PrimaryAttackProjectileClass);
    EnhancedInput->BindAction(Input_SecondaryAttack, ETriggerEvent::Triggered, this,
        &ThisClass::StartProjectileAttack, SecondaryAttackProjectileClass);
    EnhancedInput->BindAction(Input_SpecialAttack, ETriggerEvent::Triggered, this,
        &ThisClass::StartProjectileAttack, SpecialAttackProjectileClass);
}

float ARoguePlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
    AActor* DamageCauser)
{
    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    ActionSystemComponent->ApplyHealthChange(-ActualDamage);

    return ActualDamage;
}

