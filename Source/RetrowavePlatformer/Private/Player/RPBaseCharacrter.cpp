// Retrowave platformer game


#include "Player/RPBaseCharacrter.h"
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
    

ARPBaseCharacrter::ARPBaseCharacrter()
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

void ARPBaseCharacrter::BeginPlay()
{
	Super::BeginPlay();
	check(GetWorld());
    check(SpringArm);
    check(Camera);
    check(WeaponComponent);
    check(HealthComponent);
    check(GetMesh());

    HealthComponent->OnDeath.AddUObject(this, &ARPBaseCharacrter::OnDeath);
}

void ARPBaseCharacrter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    SetPlayerRotationToCursor();
}

void ARPBaseCharacrter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ARPBaseCharacrter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ARPBaseCharacrter::MoveRight);

    DECLARE_DELEGATE_OneParam(FOnSprintSignature, bool);
    PlayerInputComponent->BindAction<FOnSprintSignature>("Sprint", IE_Pressed, this, &ARPBaseCharacrter::WantsToSprint, true);
    PlayerInputComponent->BindAction<FOnSprintSignature>("Sprint", IE_Released, this, &ARPBaseCharacrter::WantsToSprint, false);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARPBaseCharacrter::Jump);

    DECLARE_DELEGATE_OneParam(FOnSetFireSignature, bool);
    PlayerInputComponent->BindAction<FOnSetFireSignature>("Fire", IE_Pressed, WeaponComponent, &URPWeaponComponent::SetWeaponStartFire, true);
    PlayerInputComponent->BindAction<FOnSetFireSignature>("Fire", IE_Released, WeaponComponent, &URPWeaponComponent::SetWeaponStartFire, false);

    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &URPWeaponComponent::TryToReload);
    PlayerInputComponent->BindAction("ChangeWeapon", IE_Pressed, WeaponComponent, &URPWeaponComponent::ChangeWeapon);

}

bool ARPBaseCharacrter::IsRunning() const
{
    return bWantsToSprint && !GetVelocity().IsZero() && FMath::IsWithin(GetMovementDirection(), -30.f, 30.f);
}

void ARPBaseCharacrter::MoveForward(float Amount)
{
    AddMovementInput(FVector(1.f, 0.f, 0.f), Amount);
}

void ARPBaseCharacrter::MoveRight(float Amount)
{
    AddMovementInput(FVector(0.f, 1.f, 0.f), Amount);
}

void ARPBaseCharacrter::WantsToSprint(bool bEnabled)
{
    bWantsToSprint = bEnabled;
}

float ARPBaseCharacrter::GetMovementDirection() const
{
    if (GetVelocity().IsZero()) return 0.f;

    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ARPBaseCharacrter::SetPlayerRotationToCursor()
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
}

void ARPBaseCharacrter::OnDeath() 
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

