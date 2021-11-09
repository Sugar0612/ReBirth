// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "MainPlayer.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// �����һ������������ص�
FVector ASpawnVolume::GetSpawnPoint() {
	FVector Extent = SpawningBox->GetScaledBoxExtent(); // ��Χ
	FVector Origin = SpawningBox->GetComponentLocation(); // box������

	// ���� random ����ҵ�
	FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	return RandomPoint;
}


// _Implementation ��Ϊ����Blueprint�Ľű������� c++��ʵ��
void ASpawnVolume::SpawnOurPawn_Implementation(UClass* ToSpawn, const FVector& Location)
{
	if (ToSpawn) {
		FActorSpawnParameters SpawnParamenter;
		UWorld* world = GetWorld();
		if (world) {
			/* �������ToSpawn ��ô����������󣬲�������������������� */
			AMainPlayer* Monster = world->SpawnActor<AMainPlayer>(ToSpawn, Location, FRotator(0.f), SpawnParamenter);
		}
	}
}

