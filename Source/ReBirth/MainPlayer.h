// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainPlayer.generated.h"

UCLASS()
class REBIRTH_API AMainPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void MoveForward(float value);
	void MoveRight(float value);
	void CameraPitch(float value);
	void CameraYaw(float value);

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UPlayerMovementComponent* OurMovementComponent;

public:
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	FORCEINLINE void SetMeshComponent(UStaticMeshComponent* Mesh) { MeshComponent = Mesh; }
	FORCEINLINE UStaticMeshComponent* GetMeshComponent() { return MeshComponent; }
	FORCEINLINE void SetSphereComponent(USphereComponent* sphere) { SphereComponent = sphere; }
	FORCEINLINE USphereComponent* GetSphereComponent() { return SphereComponent; }

	FORCEINLINE void SetCameraComponent(UCameraComponent* camera) { Camera = camera; }
	FORCEINLINE UCameraComponent* GetCameraComponent() { return Camera; }
	FORCEINLINE void SetSpringArmComponent(USpringArmComponent* arm) { SpringArm = arm; }
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() { return SpringArm; }

public:
	FVector2D CameraInput;
};
