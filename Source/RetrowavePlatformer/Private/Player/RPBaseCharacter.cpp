// Retrowave platformer game


#include "Player/RPBaseCharacter.h"
#include "Player/RPPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/RPWeaponComponent.h"
#include "Components/RPHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

ARPBaseCharacter::ARPBaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    SpringArm->SetupAttachment(GetRootComponent());
    SpringArm->TargetArmLength = 900.f;
    SpringArm->SetUsingAbsoluteRotation(true);

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    Camera->SetupAttachment(SpringArm);

    WeaponComponent = CreateDefaultSubobject<URPWeaponComponent>("WeaponComponent");

    HealthComponent = CreateDefaultSubobject<URPHealthComponent>("HealthComponent");
}

void ARPBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    check(GetWorld());
    check(SpringArm);
    check(Camera);
    check(WeaponComponent);
    check(HealthComponent);
    check(GetMesh());

    HealthComponent->OnDeath.AddUObject(this, &ARPBaseCharacter::OnDeath);
}

void ARPBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    SetPlayerRotationToCursor();
}

void ARPBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ARPBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ARPBaseCharacter::MoveRight);

    DECLARE_DELEGATE_OneParam(FOnSprintSignature, bool);
    PlayerInputComponent->BindAction<FOnSprintSignature>("Sprint", IE_Pressed, this, &ARPBaseCharacter::WantsToSprint, true);
    PlayerInputComponent->BindAction<FOnSprintSignature>("Sprint", IE_Released, this, &ARPBaseCharacter::WantsToSprint, false);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARPBaseCharacter::Jump);

    DECLARE_DELEGATE_OneParam(FOnSetFireSignature, bool);
    PlayerInputComponent->BindAction<FOnSetFireSignature>(
        "Fire", IE_Pressed, WeaponComponent, &URPWeaponComponent::SetWeaponStartFire, true);
    PlayerInputComponent->BindAction<FOnSetFireSignature>(
        "Fire", IE_Released, WeaponComponent, &URPWeaponComponent::SetWeaponStartFire, false);

    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &URPWeaponComponent::TryToReload);
    PlayerInputComponent->BindAction("ChangeWeapon", IE_Pressed, WeaponComponent, &URPWeaponComponent::ChangeWeapon);
}

bool ARPBaseCharacter::IsRunning() const
{
    bool bIsRunning = bWantsToSprint && !GetVelocity().IsZero() && FMath::IsWithin(GetMovementDirection(), -30.f, 30.f);
    bIsRunning ? 
        GetCharacterMovement()->MaxWalkSpeed = 1000.f : 
        GetCharacterMovement()->MaxWalkSpeed = 600.f;
    return bIsRunning;
}

void ARPBaseCharacter::MoveForward(float Amount)
{
    AddMovementInput(FVector(1.f, 0.f, 0.f), Amount);
}

void ARPBaseCharacter::MoveRight(float Amount)
{
    AddMovementInput(FVector(0.f, 1.f, 0.f), Amount);
}

void ARPBaseCharacter::WantsToSprint(bool bEnabled)
{
    bWantsToSprint = bEnabled;
}

float ARPBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero()) return 0.f;

    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ARPBaseCharacter::SetPlayerRotationToCursor()
{
    const auto PlayerController = GetController<ARPPlayerController>();
    if (!PlayerController) return;

    FHitResult HitResult;
    PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

    if (HitResult.bBlockingHit)
    {
        float YawValue = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitResult.ImpactPoint).Yaw;
        SetActorRotation(FRotator(0.f, YawValue, 0.f));
    }
    else
    {
        FVector2D PlayerScreenPosition, MouseScreenPosition;
        GetActorAndCursorScreenPosition(PlayerController, PlayerScreenPosition, MouseScreenPosition);

        float YawValue = UKismetMathLibrary::FindLookAtRotation(FVector(PlayerScreenPosition, 0.f), FVector(MouseScreenPosition, 0.f)).Yaw + 90.f;
        SetActorRotation(FRotator(0.f, YawValue, 0.f));
    }
}

void ARPBaseCharacter::GetActorAndCursorScreenPosition(
    APlayerController* PlayerController, FVector2D& ActorScreenPosition, FVector2D& MouseScreenPosition)
{
    if (!PlayerController || !PlayerController->GetLocalPlayer())
    {
        ActorScreenPosition = FVector2D::ZeroVector;
        MouseScreenPosition = FVector2D::ZeroVector;
        return;
    }

    PlayerController->ProjectWorldLocationToScreen(GetActorLocation(), ActorScreenPosition, true);

    PlayerController->GetLocalPlayer()->ViewportClient->GetMousePosition(MouseScreenPosition);
}

void ARPBaseCharacter::OnDeath()
{
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(5.f);
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    WeaponComponent->SetWeaponStartFire(false);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
}


