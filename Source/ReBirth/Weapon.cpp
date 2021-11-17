// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Main.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Monster.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	SkeletalComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalComponent->SetupAttachment(GetRootComponent());

	HarmBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Harm"));
	HarmBox->SetupAttachment(GetRootComponent());

	/* *����������ɵ��˺� */
	harm = 25.0f;

	/* *Ĭ������״̬: ���� */
	WeaponStatus = EWeaponStatus::EWS_Ldle;

	bParticles = true;
}

void AWeapon::BeginPlay() {
	Super::BeginPlay();

	HarmBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::WeaponOnBeginOverlap);
	HarmBox->OnComponentEndOverlap.AddDynamic(this, &AWeapon::WeaponOnEndOverlap);


	/* *��ײԤ�� */
	HarmBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HarmBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	HarmBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HarmBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor && (WeaponStatus == EWeaponStatus::EWS_Ldle)) {
		AMain* player = Cast<AMain>(OtherActor);
		if (player) {
			player->SetItemOverlap(this);
		}
	}
	
}

void AWeapon::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor) {
		AMain* player = Cast<AMain>(OtherActor);
		if (player) {
			player->SetItemOverlap(nullptr);
		}
	}
}

void AWeapon::equipWeapon(AMain* player) {
	/* *����ĳЩ���͵���ײ */
	SkeletalComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SkeletalComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SkeletalComponent->SetSimulatePhysics(false);

	/* *����������Ĳۿ� */
	const USkeletalMeshSocket* WeaponSocket = player->GetMesh()->GetSocketByName("WeaponSocket");
	if (WeaponSocket) {

		/* *�����ȡ�ɹ���ô�����ڲ���� */
		WeaponSocket->AttachActor(this, player->GetMesh());

		/* *����״̬: װ�� */
		WeaponStatus = EWeaponStatus::EWS_Equip;

		/* *ֹͣ��ת */
		is_Rotation = false;

		player->SetWeapon(this);
		player->SetItemOverlap(nullptr);
	}

	if (!bParticles) {
		/* *ȡ��������Ч */
		Particles->Deactivate();
	}

	/* *��������װ����Ч */
	if (SoundWeapon) UGameplayStatics::PlaySound2D(this, SoundWeapon);
}

void AWeapon::WeaponOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		AMonster* monster = Cast<AMonster>(OtherActor);
		if (monster) {
			if (monster->BleedParticles) {
				/* ��ѪЧ��
				* ���һ��������ʾ�Ƿ�ֻʹ��һ��
				*/

				/* *�ܵ��˺�������Ч */
				if (monster->HarmSound) UGameplayStatics::PlaySound2D(monster, monster->HarmSound);

				const USkeletalMeshSocket* WeaponSocket = SkeletalComponent->GetSocketByName("WeaponSocket");
				if (WeaponSocket) {
					FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalComponent);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), monster->BleedParticles, SocketLocation, FRotator(0.f), false);
				}
			}
		}
	}
}

void AWeapon::WeaponOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void AWeapon::OpenCollision()
{
	HarmBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::CloseCollision()
{
	HarmBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::BeginWeaponSound() {
	if (AttackSound) UGameplayStatics::PlaySound2D(this, AttackSound);
}