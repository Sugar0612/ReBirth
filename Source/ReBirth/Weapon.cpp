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

	/* *默认武器状态: 闲置 */
	WeaponStatus = EWeaponStatus::EWS_Ldle;

	/* *Attack Power */
	Attack_Power = 35.0f;

	bParticles = true;
}

void AWeapon::BeginPlay() {
	Super::BeginPlay();

	HarmBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::WeaponOnBeginOverlap);
	HarmBox->OnComponentEndOverlap.AddDynamic(this, &AWeapon::WeaponOnEndOverlap);


	/* *碰撞预设 */
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
		WeaponStatus = EWeaponStatus::EWS_Equip;

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

void AWeapon::WeaponOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		AMonster* monster = Cast<AMonster>(OtherActor);
		if (monster && WeaponStatus == EWeaponStatus::EWS_Equip) {
			if (monster->BleedParticles) {
				/* 流血效果
				* 最后一个参数表示是否只使用一次
				*/

				/* *受到伤害播放音效 */
				if (monster->HarmSound) UGameplayStatics::PlaySound2D(monster, monster->HarmSound);

				const USkeletalMeshSocket* WeaponSocket = SkeletalComponent->GetSocketByName("WeaponSocket");
				if (WeaponSocket) {
					FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalComponent);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), monster->BleedParticles, SocketLocation, FRotator(0.f), false);
				}
			}

			if (DamageTypeClass) {
				UGameplayStatics::ApplyDamage(monster, Attack_Power, nullptr, this, DamageTypeClass);
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