// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

UCLASS()
class REBIRTH_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

public:
	
	//AllowPrivateAccess: 允许私人访问(只能在蓝图内访问)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* PlayerEye;

	//添加转向速率
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float TurnRate = 65.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float LookupRate = 65.f;
	
public:
	/* 添加人物的属性 */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaxHp")
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentHp")
	float CurrentHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaxEp")
	float MaxEp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentEp")
	float CurrentEp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoinsCounts")
	int cntCoins;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	FORCEINLINE class USpringArmComponent* GetCameraArmComponent() { return CameraArm; }
	FORCEINLINE class UCameraComponent* GetPlayerEyeComponent() { return PlayerEye; }

private:
	void MoveForward(float input);
	void MoveRight(float input);
	void TurnAtRate(float input);
	void LookupAtRate(float input);
	void BeginQuicken();
	void EndQuicken();

private:
	bool is_quick = false;
};
