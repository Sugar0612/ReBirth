// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

UENUM(BlueprintType)
enum class EMonsterState : uint8 {
	EMS_Ldle UMETA(DeplayName = "Ldle"),
	EMS_MoveToTarget UMETA(DeplayName = "MoveToPlayer"),
	EMS_Attacking UMETA(DeplayName = "Attack"),
	EMS_Default UMETA(DeplayName = "Default")

};

UCLASS()
class REBIRTH_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovementState")
	EMonsterState MonsterState;

	FORCEINLINE void SetMonsterState(EMonsterState setState) { MonsterState = setState; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewComponent")
	class USphereComponent* ViewComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombetComponent")
	USphereComponent* CombetComponent;

	/* *AI¿ØÖÆÆ÷ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Controller")
	class AAIController* MonsterController;

public:

	UFUNCTION()
	void CombetBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void CombetEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "is_Overlap")
	bool bisOverlap;

};
