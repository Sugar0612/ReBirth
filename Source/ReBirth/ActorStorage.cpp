// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorStorage.h"

// Sets default values
AActorStorage::AActorStorage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AActorStorage::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorStorage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

