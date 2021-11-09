// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	/* Create Collisoin of Sphere */
	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	RootComponent = CollisionVolume;

	/* Create Mesh Component */
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	/* Particles System Component Create */
	Particles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles System"));
	Particles->SetupAttachment(GetRootComponent());

	/* Actor rotation the is_Rotation is True , False is not rotation. */
	is_Rotation = false;

	RotatorAngle = 60.0f;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnBeginOverlap);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AItem::OnEndOverlap);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	/* Actor rotation the is_Rotation is True , False is not rotation. */
	if (is_Rotation) {
		FRotator Angle = GetActorRotation();
		Angle.Yaw += (DeltaTime * RotatorAngle);
		SetActorRotation(Angle);
	}
}

void AItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlapParticles) {
		/* 附加碰撞后的粒子效果 */
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), true);
	}
	
	/* Create Overlap Actor's Sound */
	if (OverlapSound) {
		UGameplayStatics::PlaySound2D(this, OverlapSound);
	}

	/* 碰撞销毁！ */
	Destroy();
}

void AItem::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

