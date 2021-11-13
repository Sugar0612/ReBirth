// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Main.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AWeapon::AWeapon()
{
	SkeletalComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalComponent->SetupAttachment(GetRootComponent());

	/* *Ĭ������״̬: ���� */
	WeaponStatus = EWeaponStatus::EWS_Ldle;

	bParticles = true;
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
		//WeaponStatus = EWeaponStatus::EWS_Equip;

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