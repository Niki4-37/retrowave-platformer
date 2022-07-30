// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RPBaseCharacrter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ARPBaseCharacrter::ARPBaseCharacrter()
{
	PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    SpringArm->SetupAttachment(GetRootComponent());
    SpringArm->TargetArmLength = 900.f;
    SpringArm->SetUsingAbsoluteRotation(true);

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    Camera->SetupAttachment(SpringArm);

}

void ARPBaseCharacrter::BeginPlay()
{
	Super::BeginPlay();
	check(GetWorld());
    check(SpringArm);
    check(Camera);
}

void ARPBaseCharacrter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPBaseCharacrter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ARPBaseCharacrter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ARPBaseCharacrter::MoveRight);

    DECLARE_DELEGATE_OneParam(FOnSprintSignature, bool);
    PlayerInputComponent->BindAction<FOnSprintSignature>("Sprint", IE_Pressed, this, &ARPBaseCharacrter::WantsToSprint, true);
    PlayerInputComponent->BindAction<FOnSprintSignature>("Sprint", IE_Released, this, &ARPBaseCharacrter::WantsToSprint, false);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARPBaseCharacrter::Jump);

}

bool ARPBaseCharacrter::IsRunning() const
{
    return bWantsToSprint && !GetVelocity().IsZero() && FMath::IsWithin(GetMovementDirection(), -30.f, 30.f);
}

void ARPBaseCharacrter::MoveForward(float Amount)
{
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ARPBaseCharacrter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
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

