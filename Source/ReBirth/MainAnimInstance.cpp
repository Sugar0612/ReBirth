// Fill out your copyright notice in the Description page of Project Settings.

#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Main.h"

void UMainAnimInstance::InitAnimation() {
	if (!Pawn) {
		Pawn = TryGetPawnOwner();
	}
}

void UMainAnimInstance::UpdateAnimation() {
	if (!Pawn) {
		Pawn = TryGetPawnOwner();
		if (mainplayer == nullptr) {
			mainplayer = Cast<AMain>(Pawn);
		}
	}

	if (Pawn) {
		FVector Speed = Pawn->GetVelocity();
		FVector flatSpeed = FVector(Speed.X, Speed.Y, 0.0f);
		
		MovementSpeed = flatSpeed.Size(); // FMath::Sqrt(X*X + Y*Y + Z*Z);
		is_jump = Pawn->GetMovementComponent()->IsFalling();

		if (mainplayer == nullptr) {
			mainplayer = Cast<AMain>(Pawn);
		}
	}

	
}