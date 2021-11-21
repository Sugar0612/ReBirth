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
	EMS_Death UMETA(DeplayName = "Death"),
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

	/* *被伤害粒子系统 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "bleed")
	class UParticleSystem* BleedParticles;

	/* *攻击检测盒子 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack Box")
	class UBoxComponent* AttackBox;

	/* *视界盒子 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "View Box")
	class USphereComponent* ViewBox;

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
	class USphereComponent* CombetComponent;

	/* *AI控制器 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Controller")
	class AAIController* MonsterController;

	/* *被攻击时的声音 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit on Sound")
	class USoundCue* HarmSound;

	/* *攻击的声音 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Sound")
	USoundCue* AttackSound;

	/* *手的骨骼控件 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skeletal Component")
	class USkeletalMeshComponent* HandSkeletalComponent;

public:

	UFUNCTION()
	void ViewBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ViewEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void CombetBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void CombetEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void AttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void AttackEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/*UFUNCTION(BlueprintCallable)
	void GoToTarget(AMain* target);*/

public:
	
	/* *蒙太奇 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageMonster")
	UAnimMontage* MonsterMontage;

	void Attack();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaxHp")
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current Hp")
	float CurHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Power")
	float Attack_Power;

	UFUNCTION(BlueprintCallable)
	void OpenCollision();

	UFUNCTION(BlueprintCallable)
	void CloseCollision();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "is Overlap")
	bool bOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "is Attacking")
	bool bAttacking;

	UFUNCTION(BlueprintCallable)
	void EndAttack();
	
public:

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Between Time")
	float AttackBetweenTime_min;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Between Time")
	float AttackBetweenTime_max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combet")
	TSubclassOf<UDamageType> DamageTypeClass;

public:

	UFUNCTION(BlueprintCallable)
	void ReduceHp(float num);

	UFUNCTION(BlueprintCallable)
	void death();

	/* *伤害机制 */
	virtual float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:

	FTimerHandle DeathTimer;

	UFUNCTION(BlueprintCallable)
	void EndDeathMontage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	class AMain* targetPlayer;

	void DestroyActor() { this->Destroy(); }
};
