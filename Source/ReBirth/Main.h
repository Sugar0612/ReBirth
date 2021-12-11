// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8 {
	EMS_RUNING UMETA(DeplayName = "Running"),
	EMS_WALK UMETA(DeplayName = "Walking"),
	EMS_Death UMETA(DeplayName = "Death"),
	EMS_Repel UMETA(DeplayName = "Repel"),
	EMS_LDLE UMETA(DeplayName = "LDLE")
};

UENUM(BlueprintType)
/* *Ep的状态 */
enum class EEpStatus : uint8 {
	EES_Normal UMETA(DeplayName = "Normal"),
	EES_Exhaust UMETA(DeplayName = "Exhaust"),
	EES_Minmum UMETA(DeplayName = "Minmum"),
	EES_MaxEp UMETA(DeplayName = "Default Ep")
};

UCLASS()
class REBIRTH_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

public:

	/* 武器 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items | Weapon")
	class AWeapon* equipWeapon;

	/* *Save Weapon in Game */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Save Weapon")
	TSubclassOf<class AActorStorage> SaveWeapon;

	/* *装备重叠 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items | Item")
	class AItem* Overlapitem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items | Item")
	bool bisEquip;

	//Emun MovementStatus
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "status Movement")
	EMovementStatus MovementStatus;

	/* *Ep Status */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ep Status")
	EEpStatus EpStatus;

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
	
	/* *流血粒子系统 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blood at Player")
	class UParticleSystem* BloodParticles;

	/* *被伤害的音频 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound of Harm")
	class USoundCue* HarmSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Has Target Monster")
	bool HasTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Controller")
	class AMainPlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster Location")
	FVector MonsterLocation;

	virtual void Jump() override;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float Defense;

public:
	void HpReduce(float num);

	void died();

	void IncreaseCoins(float Coinnum);

	void EpReduce(float num);

	void EpRecovery(float num);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exhaust limite")
	float ExhaustLimite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minmum limite")
	float MinmumLimite;

public:
	FORCEINLINE class USpringArmComponent* GetCameraArmComponent() { return CameraArm; }
	FORCEINLINE class UCameraComponent* GetPlayerEyeComponent() { return PlayerEye; }
	FORCEINLINE void SetEpStatus(EEpStatus status) { EpStatus = status; }
	FORCEINLINE AWeapon* GetWeapon() { return equipWeapon; }
	FORCEINLINE AItem* GetItemOverlap() { return Overlapitem; }
	FORCEINLINE void SetItemOverlap(AItem* SetItem) { Overlapitem = SetItem; }
	FORCEINLINE void SetHasTarget(bool target) { HasTarget = target; }
	FORCEINLINE void SetMovementState(EMovementStatus state) { MovementStatus = state; }
private:
	void MoveForward(float input);
	void MoveRight(float input);
	void TurnAtRate(float input);
	void LookupAtRate(float input);
	void BeginQuicken();
	void EndQuicken();
	void PickUpWeapon();
	void DropWeapon();
	void AttackBegin();
	void ShowPauseWidget();
	void QuitPauseWidget();
	void GotoNextLeven();
	void _GotoNextLeven();
public:
	bool is_quick = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	bool bAttacking = false;
	
	bool bShowEsc;
	void FilpBool(bool&);
public:

	void Attack();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* RepelMontage;

public:
	//Other 一些不能分类比较特殊的函数

	/* *需要在攻击的时候不让其跳跃，所以需要在Blurprint中调用 */
	UFUNCTION(BlueprintCallable, Category = "Attack end")
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void RepelEnd();

	void SetWeapon(AWeapon* SetWeapon);


public:
	float InsterSpeed;
	bool bInsterToMonster;
	void SetInsterToMonster(bool SetInster);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inster Target")
	class AMonster* targetMonster;

	FORCEINLINE void SetInsterTarget(class AMonster* target) { targetMonster = target; }

	FRotator GetYawToMonster(FVector LocationToMonster);

	FTimerHandle DeathHandle;

	void DestroyActor();

	UFUNCTION(BlueprintCallable)
	void EndDeath();

public:

	/* *伤害机制 */
	virtual float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame(bool bLoad);

	UFUNCTION()
	bool CanMove();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items | Item")
	class ALevenSend* Leven;

	void SwitchLeven(FName nextLeven);
};
