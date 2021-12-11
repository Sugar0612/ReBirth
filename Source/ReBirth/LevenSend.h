// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevenSend.generated.h"

UCLASS()
class REBIRTH_API ALevenSend : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevenSend();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leven")
	class UBoxComponent* LevenDoor;

	class UBillboardComponent* Billboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leven Name")
	FName LevenName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
	class UParticleSystemComponent* SendDoorParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Render")
	class UTextRenderComponent* TextRender;

	class AMain* targetPlayer;

	bool binDoor;
	bool bSwitchLeven;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void LevenBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void LevenEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
