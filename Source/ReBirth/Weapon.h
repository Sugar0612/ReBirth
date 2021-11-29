// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

/**
 * 
 */

/* *ÎäÆ÷×´Ì¬ */
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
	/* *Weapon's save Name. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Save Name Of Weapon")
	FString SaveName;

	/* *ÎäÆ÷×´Ì¬ */
	EWeaponStatus WeaponStatus;

	/* *ÎäÆ÷¹Ç÷À¿Ø¼þ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeletal")
	class USkeletalMeshComponent* SkeletalComponent;

	/* *×°±¸ÎäÆ÷Ê±µÄÒôÐ§ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound on Weapon")
	class USoundCue* SoundWeapon;

	/* *ÊÇ·ñ¿ªÆôÁ£×ÓÌØÐ§ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | isParticles")
	bool bParticles;

	/* *ÉËº¦ºÐ×Ó */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | harm")
	class UBoxComponent* HarmBox;

	/* *ÎäÆ÷¹¥»÷µÄÒôÐ§ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Sound")
	class USoundCue* AttackSound;

	/* *ÎäÆ÷¹¥»÷Á¦ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float Attack_Power;

public:
	AWeapon();

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	/* *×°±¸ÎäÆ÷ */
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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combet")
	TSubclassOf<UDamageType> DamageTypeClass;
};
