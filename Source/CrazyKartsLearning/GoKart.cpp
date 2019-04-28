// Fill out your copyright notice in the Description page of Project Settings.

#include "GoKart.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"

// Sets default values
AGoKart::AGoKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector force = (GetActorForwardVector() * Throttle * MaxForce) + GetGroundAcceleration() + ApplyAirResistence();
	FVector acceleration = force / Mass;

	Velocity += acceleration * DeltaTime;

	UpdateRotation(DeltaTime);

	UpdatePositionFromVelocity(DeltaTime);
}

FVector AGoKart::GetGroundAcceleration()
{
	float gravity = -GetWorld()->GetGravityZ() / 100;
	float normalForce = gravity * Mass;

	FVector groundForce = -Velocity.GetSafeNormal() * normalForce * 0.25f;

	return groundForce;
}

FVector AGoKart::ApplyAirResistence()
{
	FVector airResistenceForce = -FMath::Square(Velocity.Size()) * GetActorForwardVector();

	return airResistenceForce * 4;
}

void AGoKart::UpdateRotation(float DeltaTime)
{
	float RotationAngle = MaxDegreesPerSecond * DeltaTime * SteeringThrow;
	FQuat RotationDelta(GetActorUpVector(), FMath::DegreesToRadians(RotationAngle));

	Velocity = RotationDelta.RotateVector(Velocity);

	AddActorWorldRotation(RotationDelta);
}

void AGoKart::UpdatePositionFromVelocity(float DeltaTime)
{
	FVector translation = Velocity * 100 * DeltaTime;

	FHitResult hitResult;

	AddActorWorldOffset(translation, true, &hitResult);

	if (hitResult.IsValidBlockingHit())
		Velocity = FVector::ZeroVector;
}

// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoKart::MoveRight);
}

void AGoKart::MoveForward(float value)
{
	Throttle = value * ThrottleMultiplier;
}

void AGoKart::MoveRight(float value)
{
	SteeringThrow = value;
}

