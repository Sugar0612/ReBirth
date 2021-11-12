// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class REBIRTH_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	/* *武器骨骼控件 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeletal")
	class USkeletalMeshComponent* SkeletalComponent;

	/* *装备武器时的音效 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound on Weapon")
	class USoundCue* SoundWeapon;

	/* *是否开启粒子特效 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | isParticles")
	bool bParticles;
public:
	AWeapon();

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	/* *装备武器 */
	void equipWeapon(class AMain* player);

};
