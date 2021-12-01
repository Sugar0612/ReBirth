// Fill out your copyright notice in the Description page of Project Settings.


#include "LevenSend.h"
#include <Components/BoxComponent.h>
#include <Components/BillboardComponent.h>
#include "Main.h"

// Sets default values
ALevenSend::ALevenSend()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	LevenDoor = CreateDefaultSubobject<UBoxComponent>(TEXT("Box leven"));
	RootComponent = LevenDoor;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Bill board"));
	Billboard->SetupAttachment(GetRootComponent());


	LevenName = "rebirth";
}

// Called when the game starts or when spawned
void ALevenSend::BeginPlay()
{
	Super::BeginPlay();
	
	LevenDoor->OnComponentBeginOverlap.AddDynamic(this, &ALevenSend::LevenBeginOverlap);
}

// Called every frame
void ALevenSend::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevenSend::LevenBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		AMain* target = Cast<AMain>(OtherActor);
		if (target) {
			target->SwitchLeven(LevenName);
		}
	}
}