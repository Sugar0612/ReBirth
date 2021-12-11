// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void AMainPlayerController::BeginPlay() {

	Super::BeginPlay();

	bCanMove = false;

	/* 如果你调用了 HUD的模板类 那么将HUDWidget 输出到平面上 */
	if (OverlayWidgetAsset) {
		/* Create HUDWidget */
		HUDOverlay = CreateWidget<UUserWidget>(this, OverlayWidgetAsset);
	}

	/* 输出到视口上面去 */
	HUDOverlay->AddToViewport();

	//可以将特定的窗口输出到视口上面.
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	if (MonsterHpWidget) {
		MonsterHpBar = CreateWidget<UUserWidget>(this, MonsterHpWidget);
		if (MonsterHpBar) {
			MonsterHpBar->AddToViewport();
			MonsterHpBar->SetVisibility(ESlateVisibility::Hidden);
		}
		FVector2D Alignment(0.f, 0.f);
		MonsterHpBar->SetAlignmentInViewport(Alignment);
	}


	if (PauseWidget) {
		PauseWidgetBar = CreateWidget<UUserWidget>(this, PauseWidget);
		if (PauseWidgetBar) {
			PauseWidgetBar->AddToViewport();
			PauseWidgetBar->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (BeginPlayWidget) {
		BeginPlayBar = CreateWidget<UUserWidget>(this, BeginPlayWidget);
		if (BeginPlayBar) {
			BeginPlayBar->AddToViewport();
			BeginPlayBar->SetVisibility(ESlateVisibility::Visible);
		}
	}

}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MonsterHpBar) {
		FVector2D ScreenPosition;

		/* *将3d转换为屏幕2d坐标 */
		ProjectWorldLocationToScreen(showLocation, ScreenPosition);

		FVector2D ProgressBarSize(200.f, 25.f);

		MonsterHpBar->SetPositionInViewport(ScreenPosition);
		MonsterHpBar->SetDesiredSizeInViewport(ProgressBarSize);
	}
}


void AMainPlayerController::ShowHpBar() {
	if (MonsterHpBar) {
		bshowHpBar = true;
		MonsterHpBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::HideHpBar() {
	if (MonsterHpBar) {
		bshowHpBar = false;
		MonsterHpBar->SetVisibility(ESlateVisibility::Hidden);
	}
}


void AMainPlayerController::ShowPauseBar_Implementation() {
	UWorld* World = GetWorld();
	UGameplayStatics::SetGamePaused(World, true);

	if (PauseWidgetBar) {
		bshowPauseBar = true;
		PauseWidgetBar->SetVisibility(ESlateVisibility::Visible);

		/* *打开鼠标的交互 */
		FInputModeGameAndUI InputModeGameAndUI;
		SetInputMode(InputModeGameAndUI);

		/* *显示鼠标 */
		bShowMouseCursor = true;
	}
}

void AMainPlayerController::HidePauseBar_Implementation() {
	UWorld* World = GetWorld();
	UGameplayStatics::SetGamePaused(World, false);

	if (PauseWidgetBar) {
		bshowPauseBar = false;
		PauseWidgetBar->SetVisibility(ESlateVisibility::Hidden);

		/* *关闭鼠标的交互 */

		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);

		/* *关闭鼠标 */
		bShowMouseCursor = false;
	}
}

void AMainPlayerController::TogglePauseWidget()
{
	if (!bshowPauseBar) {
		ShowPauseBar();
	}
	else {
		HidePauseBar();
	}
}

void AMainPlayerController::BeginPlayGame()
{
	if (BeginPlayBar) {
		BeginPlayBar->SetVisibility(ESlateVisibility::Hidden);
		bCanMove = true;
	}
}

void AMainPlayerController::QuitPlayGame() {

}