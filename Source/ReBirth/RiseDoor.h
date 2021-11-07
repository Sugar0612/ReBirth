// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RiseDoor.generated.h"

UCLASS()
class REBIRTH_API ARiseDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARiseDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	/* 碰撞盒子 触碰触发事件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
	class UBoxComponent* TriggerBox;

	/* 地板开关按钮 */
 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
	class UStaticMeshComponent* floorSwitch;

	/* 按下按钮打开门 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
	class UStaticMeshComponent* Door;

	/* 门上的装饰 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
	class UStaticMeshComponent* decorate;
public:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Rise Door")
	void RiseTheDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Lower Door")
	void LowerTheDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Rise Switch")
	void RiseTheSwitch();

	UFUNCTION(BlueprintImplementableEvent, Category = "Lower Switch")
	void LowerTheSwitch();

	UFUNCTION(BlueprintCallable, Category = "Update Door")
	void UpdateDoorLocation(float z);

	UFUNCTION(BlueprintCallable, Category = "Update Switch")
	void UpdateSwitchLocation(float z);

public:
	FVector InitDoorLocation;
	FVector InitSwitchLocation;
};
