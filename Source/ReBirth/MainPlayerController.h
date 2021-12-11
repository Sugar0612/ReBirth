// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class REBIRTH_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> OverlayWidgetAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> MonsterHpWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> PauseWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> BeginPlayWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	class UUserWidget* HUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* MonsterHpBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* PauseWidgetBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* BeginPlayBar;

	bool bshowHpBar;
	void ShowHpBar();
	void HideHpBar();

	bool bshowPauseBar;
	bool bCanMove;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void ShowPauseBar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void HidePauseBar();

	UFUNCTION(BlueprintCallable)
	void BeginPlayGame();

	UFUNCTION(BlueprintCallable)
	void QuitPlayGame();

	void TogglePauseWidget();

	FVector showLocation;

	void CloseMouseGame();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

};
