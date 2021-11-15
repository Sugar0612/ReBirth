// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

/**
 * 
 */

/* *����״̬ */
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
	/* *����״̬ */
	EWeaponStatus WeaponStatus;

	/* *���������ؼ� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeletal")
	class USkeletalMeshComponent* SkeletalComponent;

	/* *װ������ʱ����Ч */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound on Weapon")
	class USoundCue* SoundWeapon;

	/* *�Ƿ���������Ч */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | isParticles")
	bool bParticles;
public:
	AWeapon();

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	/* *װ������ */
	void equipWeapon(class AMain* player);

public:
	FORCEINLINE void SetWeaponStatus(EWeaponStatus Status) { WeaponStatus = Status; }
	FORCEINLINE EWeaponStatus GetWeaponStatus() { return WeaponStatus; }
};
