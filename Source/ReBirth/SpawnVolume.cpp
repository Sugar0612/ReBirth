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

// 随机找一个怪物的重生地点
FVector ASpawnVolume::GetSpawnPoint() {
	FVector Extent = SpawningBox->GetScaledBoxExtent(); // 范围
	FVector Origin = SpawningBox->GetComponentLocation(); // box的中心

	// 利用 random 随机找点
	FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	return RandomPoint;
}


// _Implementation 是为了让Blueprint的脚本可以在 c++中实现
void ASpawnVolume::SpawnOurPawn_Implementation(UClass* ToSpawn, const FVector& Location)
{
	if (ToSpawn) {
		FActorSpawnParameters SpawnParamenter;
		UWorld* world = GetWorld();
		if (world) {
			/* 如果存在ToSpawn 那么生成世界对象，并在世界中生成这个怪物 */
			AMainPlayer* Monster = world->SpawnActor<AMainPlayer>(ToSpawn, Location, FRotator(0.f), SpawnParamenter);
		}
	}
}

