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

	/* *默认武器状态: 闲置 */
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
	/* *忽略某些类型的碰撞 */
	SkeletalComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SkeletalComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SkeletalComponent->SetSimulatePhysics(false);

	/* *获得拿武器的槽口 */
	const USkeletalMeshSocket* WeaponSocket = player->GetMesh()->GetSocketByName("WeaponSocket");
	if (WeaponSocket) {

		/* *如果获取成功那么附加在插槽上 */
		WeaponSocket->AttachActor(this, player->GetMesh());

		/* *武器状态: 装备 */
		//WeaponStatus = EWeaponStatus::EWS_Equip;

		/* *停止旋转 */
		is_Rotation = false;

		player->SetWeapon(this);
		player->SetItemOverlap(nullptr);
	}

	if (!bParticles) {
		/* *取消粒子特效 */
		Particles->Deactivate();
	}

	/* *播放武器装备音效 */
	if (SoundWeapon) UGameplayStatics::PlaySound2D(this, SoundWeapon);
}