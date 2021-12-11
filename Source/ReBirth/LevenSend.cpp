// Fill out your copyright notice in the Description page of Project Settings.


#include "LevenSend.h"
#include <Components/BoxComponent.h>
#include <Components/BillboardComponent.h>
#include "Main.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ALevenSend::ALevenSend()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LevenDoor = CreateDefaultSubobject<UBoxComponent>(TEXT("Box leven"));
	RootComponent = LevenDoor;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Bill board"));
	Billboard->SetupAttachment(GetRootComponent());

	SendDoorParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Send ParticleSystem"));
	SendDoorParticle->SetupAttachment(GetRootComponent());

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Render"));
	TextRender->SetupAttachment(GetRootComponent());
	TextRender->SetVisibility(false);

	binDoor = false;

	bSwitchLeven = false;

	LevenName = "rebirth";
}

// Called when the game starts or when spawned
void ALevenSend::BeginPlay()
{
	Super::BeginPlay();
	
	LevenDoor->OnComponentBeginOverlap.AddDynamic(this, &ALevenSend::LevenBeginOverlap);
	LevenDoor->OnComponentEndOverlap.AddDynamic(this, &ALevenSend::LevenEndOverlap);
}

// Called every frame
void ALevenSend::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (binDoor && bSwitchLeven) {
		targetPlayer->SwitchLeven(LevenName);
	}

}

void ALevenSend::LevenBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		targetPlayer = Cast<AMain>(OtherActor);
		if (targetPlayer) {
			binDoor = true;
			targetPlayer->Leven = this;
			TextRender->SetVisibility(true);
		}
	}
}


void ALevenSend::LevenEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor) {
		if (targetPlayer) {
			targetPlayer->Leven = nullptr;
		}

		if (TextRender) {
			TextRender->SetVisibility(false);
		}

		binDoor = false;
		bSwitchLeven = false;
	}
}