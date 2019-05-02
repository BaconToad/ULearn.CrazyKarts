// Fill out your copyright notice in the Description page of Project Settings.

#include "GoKart.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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

	DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(Role), this, FColor::White, DeltaTime);
}

FString AGoKart::GetEnumText(ENetRole role)
{
	switch (role)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	default:
		return "Error";
	}
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
	float radius = 20;
	float translation = FVector::DotProduct(GetActorForwardVector(), CalcTranslation(DeltaTime));
	float dAngle = (translation / radius) * SteeringThrow;

	//float RotationAngle = MaxDegreesPerSecond * DeltaTime * SteeringThrow;
	FQuat RotationDelta(GetActorUpVector(), FMath::DegreesToRadians(dAngle));

	Velocity = RotationDelta.RotateVector(Velocity);

	AddActorWorldRotation(RotationDelta);
}

void AGoKart::UpdatePositionFromVelocity(float DeltaTime)
{
	FVector translation = CalcTranslation(DeltaTime);

	FHitResult hitResult;

	AddActorWorldOffset(translation, true, &hitResult);

	if (hitResult.IsValidBlockingHit())
		Velocity = FVector::ZeroVector;
}

FVector AGoKart::CalcTranslation(float DeltaTime)
{
	return Velocity * 100 * DeltaTime;
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
	Server_MoveForward(value);
	Throttle = value * ThrottleMultiplier;
}

void AGoKart::MoveRight(float value)
{
	Server_MoveRight(value);
	SteeringThrow = value;
}

void AGoKart::Server_MoveForward_Implementation(float value)
{
	Throttle = value * ThrottleMultiplier;
}

bool AGoKart::Server_MoveForward_Validate(float value)
{
	return FMath::Abs(value) <= 1;
}

void AGoKart::Server_MoveRight_Implementation(float value)
{
	SteeringThrow = value;
}

bool AGoKart::Server_MoveRight_Validate(float value)
{
	return FMath::Abs(value) <= 1;
}