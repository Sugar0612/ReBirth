// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

/**
 * 
 */

/* *武器状态 */
UENUM(BlueprintType)
enum class EWeaponStatus : uint8 {
	EWS_Ldle UMETA(DeplayName = "Ldle"),
	EWS_Equip UMETA(DeplayName = "Equip"),
	EWS_Default UMETA(DeplayName = "Default")
};

UCLASS()
class REBIRTH_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	/* *武器状态 */
	EWeaponStatus WeaponStatus;

	/* *武器骨骼控件 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeletal")
	class USkeletalMeshComponent* SkeletalComponent;

	/* *装备武器时的音效 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound on Weapon")
	class USoundCue* SoundWeapon;

	/* *是否开启粒子特效 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | isParticles")
	bool bParticles;

	/* *伤害盒子 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | harm")
	class UBoxComponent* HarmBox;

	/* *武器的伤害 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HarmNum")
	float harm;

	/* *武器攻击的音效 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Sound")
	class USoundCue* AttackSound;

public:
	AWeapon();

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	/* *装备武器 */
	void equipWeapon(class AMain* player);

	UFUNCTION()
	void WeaponOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void WeaponOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void OpenCollision();

	UFUNCTION(BlueprintCallable)
	void CloseCollision();

	UFUNCTION(BlueprintCallable)
	void BeginWeaponSound();

public:
	FORCEINLINE void SetWeaponStatus(EWeaponStatus Status) { WeaponStatus = Status; }
	FORCEINLINE EWeaponStatus GetWeaponStatus() { return WeaponStatus; }

protected:
	virtual void BeginPlay() override;
};
