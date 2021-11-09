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



}