// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Main.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* 如果在范围内会重复攻击 */
	bOverlap = false;

	CombetComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Combet"));
	CombetComponent->SetupAttachment(GetRootComponent());
	//CombetComponent->InitSphereRadius(75.0f);


	/* Attack Box */
	AttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Detect"));
	AttackBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AttackSocket"));

	bAttacking = false;

	/* *攻击间隔时间 */
	AttackBetweenTime_min = 0.5f;
	AttackBetweenTime_max = 2.5f;

	/* *属性 */
	MaxHp = 100.0f;
	CurHp = 100.f;
	Defense = 5.f;
	Attack_Power = 15.f;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	/* *get Monster Controller */
	MonsterController = Cast<AAIController>(GetController());

	/* trigger Box Bind Function */

	CombetComponent->OnComponentBeginOverlap.AddDynamic(this, &AMonster::CombetBeginOverlap);
	CombetComponent->OnComponentEndOverlap.AddDynamic(this, &AMonster::CombetEndOverlap);

	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &AMonster::AttackBeginOverlap);
	AttackBox->OnComponentEndOverlap.AddDynamic(this, &AMonster::AttackEndOverlap);

	/* *碰撞预设 */
	AttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AttackBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonster::CombetBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;

	if (OtherActor && (MonsterState != EMonsterState::EMS_Death)) {
		AMain* Player = Cast<AMain>(OtherActor);
		if (Player) {
			/* *插值传入 */
			Player->SetInsterTarget(this);
			bOverlap = true;
			Attack();
		}
	}
}


void AMonster::CombetEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (MonsterState != EMonsterState::EMS_Death)) {
		AMain* target = Cast<AMain>(OtherActor);
		if (target) {
			/* *插值切断 */
			target->SetInsterTarget(nullptr);
			bOverlap = false;
			/* *当离开攻击范围初始化Timer */
			GetWorldTimerManager().ClearTimer(AttackTimer);
			SetMonsterState(EMonsterState::EMS_MoveToTarget);
		}
	}
}

void AMonster::Attack() {
	if (bAttacking) return;

	bAttacking = false;

	if (MonsterController) {
		MonsterController->StopMovement();
		SetMonsterState(EMonsterState::EMS_Attacking);
	}

	UAnimInstance* MonsterAnim = GetMesh()->GetAnimInstance();
	if (MonsterAnim && MonsterMontage) {
		int SectionRandom = FMath::RandRange(1, 2);
		switch (SectionRandom) {
		case 1:
			MonsterAnim->Montage_Play(MonsterMontage, 1.30f);
			MonsterAnim->Montage_JumpToSection(FName("Attack1"), MonsterMontage);
			break;
		case 2:
			MonsterAnim->Montage_Play(MonsterMontage, 1.30f);
			MonsterAnim->Montage_JumpToSection(FName("Attack2"), MonsterMontage);
			break;
		default:
			break;
		};
	}
}

void AMonster::EndAttack() {
	bAttacking = false;
	if (bOverlap) {
		float AttackBetweenTime = FMath::RandRange(AttackBetweenTime_min, AttackBetweenTime_max);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AMonster::Attack, AttackBetweenTime);
	}
}

void AMonster::ReduceHp(float num)
{
	this->CurHp -= num;
	if (CurHp <= 0.f) {
		death();
	}
}

void AMonster::death() {
	UAnimInstance* MonsterAnim = GetMesh()->GetAnimInstance();
	if (MonsterAnim && MonsterMontage) {
		MonsterState = EMonsterState::EMS_Death;
		MonsterAnim->Montage_Play(MonsterMontage, 1.3f);
		MonsterAnim->Montage_JumpToSection("death", MonsterMontage);
	}

	/* *解除碰撞 */
	CombetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMonster::EndDeathMontage()
{
	float DeathTime = 3.0f;
	//GetWorldTimerManager().SetTimer(DeathTimer, this, &AMonster::DestroyActor, DeathTime);
}

float AMonster::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	ReduceHp(DamageTaken);

	return DamageTaken;
}


void AMonster::AttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		AMain* player = Cast<AMain>(OtherActor);
		if (player) {
			if (player->BloodParticles) {
				/* 流血效果
				* 最后一个参数表示是否只使用一次
				*/

				/* *受到伤害播放音效 */
				if (player->HarmSound) UGameplayStatics::PlaySound2D(player, player->HarmSound);

				const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName("BloodSocket");
				//UE_LOG(LogTemp, Warning, TEXT(" Blood Socket Begin Blood! "));
				if (HandSocket) {
					FVector SocketLocation = HandSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), player->BloodParticles, SocketLocation, FRotator(0.f), false);
				}
			}

			if (DamageTypeClass) {
				UGameplayStatics::ApplyDamage(player, Attack_Power, MonsterController, this, DamageTypeClass);
			}
		}
	}
}

void AMonster::AttackEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}


void AMonster::OpenCollision()
{
	if (MonsterController) {
		MonsterController->StopMovement();
		AttackBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AMonster::CloseCollision()
{
	AttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

