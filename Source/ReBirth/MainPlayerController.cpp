// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay() {

	Super::BeginPlay();

	/* ���������� HUD��ģ���� ��ô��HUDWidget �����ƽ���� */
	if (OverlayWidgetAsset) {
		/* Create HUDWidget */
		HUDOverlay = CreateWidget<UUserWidget>(this, OverlayWidgetAsset);
	}

	/* ������ӿ�����ȥ */
	HUDOverlay->AddToViewport();

	//���Խ��ض��Ĵ���������ӿ�����.
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

		/* *��3dת��Ϊ��Ļ2d���� */
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