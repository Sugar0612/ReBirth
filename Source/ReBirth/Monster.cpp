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
#include "Components/SphereComponent.h"
#include "MainPlayerController.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* ����ڷ�Χ�ڻ��ظ����� */
	bOverlap = false;

	CombetComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Combet"));
	CombetComponent->SetupAttachment(GetRootComponent());
	/*CombetComponent->InitSphereRadius(75.0f);*/


	/* Attack Box */
	AttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Detect"));
	AttackBox->SetupAttachment(GetMesh(), FName("AttackSocket"));
	//AttackBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AttackSocket"));


	/* View Box */
	ViewBox = CreateDefaultSubobject<USphereComponent>(TEXT("View  Limite"));
	ViewBox->SetupAttachment(GetRootComponent());
	ViewBox->InitSphereRadius(4000.f);

	bAttacking = false;

	/* *�������ʱ�� */
	AttackBetweenTime_min = 1.0f;
	AttackBetweenTime_max = 2.0f;

	MonsterState = EMonsterState::EMS_Ldle;

	/* *���� */
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
	
	ViewBox->OnComponentBeginOverlap.AddDynamic(this, &AMonster::ViewBeginOverlap);
	ViewBox->OnComponentEndOverlap.AddDynamic(this, &AMonster::ViewEndOverlap);

	/* *��ײԤ�� */
	AttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AttackBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	/*��ײԤ��*/
	
	ViewBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAttacking) {
		if (MonsterController) {
			MonsterController->StopMovement();
		}
	}

	if (targetPlayer) {
		if (targetPlayer->PlayerController->bshowPauseBar) {
			UE_LOG(LogTemp, Warning, TEXT("target HUD is true!"));
			MonsterController->StopMovement();
		}
	}
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonster::ViewBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor) {
		targetPlayer = Cast<AMain>(OtherActor);
	}
}

void AMonster::ViewEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	/*if (OtherActor) {
		AMain* player = Cast<AMain>(OtherActor);
		if (player) {
			if (player->PlayerController) {
				player->PlayerController->HideHpBar();
			}
		}
	}*/
}

void AMonster::CombetBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;

	if (OtherActor && (MonsterState != EMonsterState::EMS_Death)) {
		AMain* Player = Cast<AMain>(OtherActor);
		if (Player) {
			targetPlayer = Player;

			if (Player->PlayerController) {
				Player->PlayerController->ShowHpBar();
			}

			/* *��ֵ���� */
			Player->SetInsterTarget(this);
			Player->SetHasTarget(true);
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
			targetPlayer = target;

			if (target->PlayerController) {
				target->PlayerController->HideHpBar();
			}

			/* *��ֵ�ж� */
			if (target->targetMonster == this) {
				target->SetInsterTarget(nullptr);
				target->SetHasTarget(false);
			}
			bOverlap = false;

			/* *���뿪������Χ��ʼ��Timer */
			GetWorldTimerManager().ClearTimer(AttackTimer);
			SetMonsterState(EMonsterState::EMS_MoveToTarget);
		}
	}
}

void AMonster::Attack() {

	bAttacking = true;
	if (MonsterState == EMonsterState::EMS_Death) return;

	SetMonsterState(EMonsterState::EMS_Attacking);
	if (MonsterController && (MonsterState != EMonsterState::EMS_Death)) {
		MonsterController->StopMovement();
		SetMonsterState(EMonsterState::EMS_Attacking);
	}

	UAnimInstance* MonsterAnim = GetMesh()->GetAnimInstance();
	if (MonsterAnim && MonsterMontage && (MonsterState == EMonsterState::EMS_Attacking)) {
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
	else {
		SetMonsterState(EMonsterState::EMS_MoveToTarget);
	}
}

void AMonster::ReduceHp(float num)
{
	if(num - Defense >= 0) this->CurHp -= (num - Defense);
	if (CurHp <= 0.f) {
		SetMonsterState(EMonsterState::EMS_Death);
		death();
	}
}

void AMonster::death() {
	UAnimInstance* MonsterAnim = GetMesh()->GetAnimInstance();
	MonsterController->StopMovement();
	if (MonsterAnim && MonsterMontage) {
		MonsterAnim->Montage_Play(MonsterMontage, 1.3f);
		MonsterAnim->Montage_JumpToSection("death", MonsterMontage);

		if (targetPlayer) {
			targetPlayer->PlayerController->HideHpBar();
			targetPlayer = nullptr;
		}
	}

	/* *�����ײ */
	/*ViewBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
	CombetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void AMonster::EndDeathMontage()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	float DeathTime = 3.0f;
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AMonster::DestroyActor, DeathTime);
}

void AMonster::DestroyActor() {
	GetMesh()->DestroyComponent(true);
}

float AMonster::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	SetMonsterState(EMonsterState::EMS_Repel);
	UAnimInstance* MonsterAnim = GetMesh()->GetAnimInstance();
	if (MonsterAnim && MonsterMontage) {
		MonsterAnim->Montage_Play(MonsterMontage, 1.3f);
		MonsterAnim->Montage_JumpToSection(FName("repel"), MonsterMontage);
	}
	GetWorldTimerManager().ClearTimer(AttackTimer);
	ReduceHp(DamageTaken);

	return DamageTaken;
}

void AMonster::EndRepel() {
	
	Attack();
}

void AMonster::AttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		UE_LOG(LogTemp, Warning, TEXT("Begin Attack! "));
		AMain* player = Cast<AMain>(OtherActor);
		if (player) {
			UE_LOG(LogTemp, Warning, TEXT("This is Player! "));
			if (player->BloodParticles) {
				/* ��ѪЧ��
				* ���һ��������ʾ�Ƿ�ֻʹ��һ��
				*/

				/* *�ܵ��˺�������Ч */
				UE_LOG(LogTemp, Warning, TEXT("Harm"));
				if (player->HarmSound) UGameplayStatics::PlaySound2D(player, player->HarmSound);

				const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName("BloodSocket");
				//UE_LOG(LogTemp, Warning, TEXT(" Blood Socket Begin Blood! "));
				if (HandSocket) {
					FVector SocketLocation = HandSocket->GetSocketLocation(GetMesh());

					/* *����ϵͳ���� */
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), player->BloodParticles, SocketLocation, FRotator(0.f), false);
				}
			}



			if (DamageTypeClass) {
				player->SetMovementState(EMovementStatus::EMS_Repel);
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