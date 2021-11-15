// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8 {
	EMS_RUNING UMETA(DeplayName = "Running"),
	EMS_WALK UMETA(DeplayName = "Walking")
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items | Weapon")
	class AWeapon* equipWeapon;

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
public:
	bool is_quick = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	bool bAttacking = false;

public:

	void Attack();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

public:
	//Other 一些不能分类比较特殊的函数

	/* *需要在攻击的时候不让其跳跃，所以需要在Blurprint中调用 */
	UFUNCTION(BlueprintCallable, Category = "Attack end")
	void AttackEnd();

	void SetWeapon(AWeapon* SetWeapon);
};
