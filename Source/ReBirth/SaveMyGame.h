// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveMyGame.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCharacterState {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float CurHp;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxHp;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float CurEp;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxEp;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int32 CoinCnt;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FString SName;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FVector Location;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FRotator Rotation;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameMap")
	FName MapName;
};

UCLASS()
class REBIRTH_API USaveMyGame : public USaveGame
{

	GENERATED_BODY()

public:

	USaveMyGame();

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	FString GameName;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	uint32 PlayerIndex;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	FCharacterState CharacterState;
};
