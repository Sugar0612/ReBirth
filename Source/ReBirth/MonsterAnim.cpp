// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnim.h"
#include "Monster.h"

void UMonsterAnim::InitAnimation() {
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn != nullptr) {
			monster = Cast<AMonster>(Pawn);
		}
	}
}

void UMonsterAnim::UpdateAnimation() {
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn != nullptr) {
			monster = Cast<AMonster>(Pawn);
		}
	}

	if (Pawn) {
		FVector Speed = Pawn->GetVelocity();
		FVector flatSpeed = FVector(Speed.X, Speed.Y, 0.0f);
		MovementSpeed = flatSpeed.Size(); // FMath::Sqrt(X*X + Y*Y + Z*Z);
		if (Pawn) {
			monster = Cast<AMonster>(Pawn);
		}
	}
}