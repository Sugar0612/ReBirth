// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Main.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* 如果在范围内会重复攻击 */
	bisOverlap = false;

	CombetComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Combet"));
	CombetComponent->SetupAttachment(GetRootComponent());
	CombetComponent->InitSphereRadius(75.0f);
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	/* *get Monster Controller */
	MonsterController = Cast<AAIController>(GetController());

	/* trigger Box Bind Function */

	CombetComponent->OnComponentBeginOverlap.AddDynamic(this, &AMonster::CombetBeginOverlap);
	CombetComponent->OnComponentEndOverlap.AddDynamic(this, &AMonster::CombetEndOverlap);
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonster::CombetBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		AMain* Player = Cast<AMain>(OtherActor);
		if (Player) {
			if (MonsterController) {
				bisOverlap = true;
				SetMonsterState(EMonsterState::EMS_Attacking);
			}
		}
	}

}


void AMonster::CombetEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor) {
		AMain* target = Cast<AMain>(OtherActor);
		if (target) {
			bisOverlap = false;
			SetMonsterState(EMonsterState::EMS_MoveToTarget);
		}
	}
}
