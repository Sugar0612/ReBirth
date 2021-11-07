// Fill out your copyright notice in the Description page of Project Settings.


#include "RiseDoor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ARiseDoor::ARiseDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	RootComponent = TriggerBox;

	/* ������ײ���� */
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //ֻ���ص��¼�
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic); // ����ײ������ World Static
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); //������������
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); // ��Pawn���� ���ó��ص�����

	TriggerBox->SetBoxExtent(FVector(62.f, 62.f, 32.f));

	floorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("floor Switch"));
	floorSwitch->SetupAttachment(GetRootComponent());

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(GetRootComponent());

	

}

// Called when the game starts or when spawned
void ARiseDoor::BeginPlay()
{
	Super::BeginPlay();
	
	/* trigger Box Bind Function */
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ARiseDoor::BeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ARiseDoor::EndOverlap);


	/* ��ʼ���� �� ���ص�λ�� */
	InitDoorLocation = Door->GetComponentLocation();
	InitSwitchLocation = floorSwitch->GetComponentLocation();
}

// Called every frame
void ARiseDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ARiseDoor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	RiseTheDoor();
	LowerTheSwitch();
}

void ARiseDoor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	//LowerTheDoor();
	//RiseTheSwitch();
}


/* ������TargetComponent->GetComponentLocation() ���� InitTargetLocation */

void ARiseDoor::UpdateDoorLocation(float z)
{
	FVector DoorLocation = InitDoorLocation;
	DoorLocation.Z += z;
	Door->SetWorldLocation(DoorLocation);
}

void ARiseDoor::UpdateSwitchLocation(float z)
{
	FVector SwitchLocation = InitSwitchLocation;
	SwitchLocation.Z -= z;
	floorSwitch->SetWorldLocation(SwitchLocation);
}