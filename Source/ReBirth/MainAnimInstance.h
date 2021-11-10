// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class REBIRTH_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	//初始化动画 类似于 BeginPlay()
	virtual void InitAnimation();

	//Tick in Animation
	UFUNCTION(BlueprintCallable, Category = Animations)
	void UpdateAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool is_jump;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class AMain* mainplayer;
};
