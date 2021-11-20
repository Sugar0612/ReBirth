// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay() {

	Super::BeginPlay();

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
			MonsterHpBar->SetVisibility(ESlateVisibility::Visible);
		}
		FVector2D Alignment(0.f, 0.f);
		MonsterHpBar->SetAlignmentInViewport(Alignment);
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