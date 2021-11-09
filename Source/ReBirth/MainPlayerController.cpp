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



}