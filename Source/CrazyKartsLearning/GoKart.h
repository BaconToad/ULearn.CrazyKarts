// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

UCLASS()
class CRAZYKARTSLEARNING_API AGoKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGoKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FVector Velocity;
	float Throttle;
	float SteeringThrow;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveForward(float value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveRight(float value);

	void UpdatePositionFromVelocity(float DeltaTime);
	FVector CalcTranslation(float DeltaTime);
	void UpdateRotation(float DeltaTime);
	FVector ApplyAirResistence();
	FVector GetGroundAcceleration();

	UPROPERTY(EditAnywhere)
		float Mass = 1000;

	UPROPERTY(EditAnywhere)
		float ThrottleMultiplier = 10;

	UPROPERTY(EditAnywhere)
		float MaxForce = 1000;

	UPROPERTY(EditAnywhere)
		float MaxDegreesPerSecond = 90;
};
