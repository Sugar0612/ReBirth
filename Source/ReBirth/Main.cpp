// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "Monster.h"
#include "Weapon.h"
#include "MainPlayerController.h"
#include "SaveMyGame.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "ActorStorage.h"
#include "LevenSend.h"

// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Camera Arm
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(GetRootComponent());
	CameraArm->TargetArmLength = 400.f;
	CameraArm->bUsePawnControlRotation = true; // ϣ�����ɸ˿��Ը�������˶�

	
	// PlayerEyes
	PlayerEye = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerEye"));
	PlayerEye->SetupAttachment(CameraArm, USpringArmComponent::SocketName); // ������Ҫ�� ���ɸ�����һ�����������һ�� ���
	PlayerEye->bUsePawnControlRotation = false; // ���ǲ�ϣ��������Ե�����ת����ֻҪ���ŵ��ɸ˾ͺ�

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; //�����ƶ������ɫ����ĳ�����ת...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f); //...ֻ��תYaw�ᣨѧ���ˣ���
	GetCharacterMovement()->JumpZVelocity = 500.f; // ��Ծ�ٶ�
	
	/*  *����Ϊ�ڿ����ƶ��ǲ������, �����Ҳ�û�����
		*GetCharacterMovement()->AirControl= 0.4f; */


	/* ��ʼ������ */
	MaxHp = 100.f;
	CurrentHp = 100.f;
	MaxEp = 100.f;
	CurrentEp = 100.f;
	cntCoins = 0.f;
	Defense = 10.f;

	/* Init Enums */
	MovementStatus = EMovementStatus::EMS_LDLE;
	EpStatus = EEpStatus::EES_Normal;

	/*��Ep С��ExhaustLimite ����MinmumLimiteΪ��ɫ����EpС�� MinmumLimiteʱΪ��ɫ*/
	ExhaustLimite = 0.6f;
	MinmumLimite = 0.25f;

	/* *�Ƿ�װ�������� */
	bisEquip = false;

	/* *��ֵ�ٶ� */
	InsterSpeed = 0.8f;
	bInsterToMonster = false;

	/* *�Ƿ��ж�ս��ɫ */
	HasTarget = false;

	/* *�Ƿ���ʾ��ͣ�˵� */
	bShowEsc = false;

	//��ȡ��ҿ�����
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	
	//������
	PlayerController = Cast<AMainPlayerController>(GetController());
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	if (is_quick && CurrentEp > 0.f) {
		/* Player pressed left Shift add Walk speed... */
		float speed = this->GetCharacterMovement()->GetMaxSpeed();
		this->GetCharacterMovement()->MaxWalkSpeed = (speed + 10.0f <= 600) ? speed + 100.0f : speed;
		EpReduce(0.2);

		if ((CurrentEp / MaxEp) <= 0.6f && (CurrentEp / MaxEp) > 0.25f) SetEpStatus(EEpStatus::EES_Exhaust);
		else if ((CurrentEp / MaxEp) <= 0.25f) SetEpStatus(EEpStatus::EES_Minmum);
		else SetEpStatus(EEpStatus::EES_Normal);

	}
	else {
		/* ...Released the Walk speed updata 230.0f */
		this->GetCharacterMovement()->MaxWalkSpeed = 360.0f;
		EpRecovery(0.08);

		if ((CurrentEp / MaxEp) >= 0.25f && (CurrentEp / MaxEp) < 0.6f) SetEpStatus(EEpStatus::EES_Exhaust);
		else if ((CurrentEp / MaxEp) >=  0.6f) SetEpStatus(EEpStatus::EES_Normal);
		else SetEpStatus(EEpStatus::EES_Minmum);
	}


	/* ʵ�ֵ����﹥����ʱ�򲻿����ƶ�...
	*...ֻ�õ���ʼ���� ���Ҳ�����ɹ�������֮��...
	*...�ſ����ƶ� 
	*/
	if(bAttacking) AttackEnd();

	if (bInsterToMonster && targetMonster) {
		FRotator LookAtRotator = GetYawToMonster(targetMonster->GetActorLocation());
		FRotator InsterRotator = FMath::RInterpTo(GetActorRotation(), LookAtRotator, DeltaTime, InsterSpeed);
		SetActorRotation(InsterRotator); 
	}
	
	if (targetMonster && (targetMonster->MonsterState != EMonsterState::EMS_Death)) {
		MonsterLocation = targetMonster->GetActorLocation();
		if (PlayerController) {
			MonsterLocation.Z += 125.f;
			MonsterLocation.X += 50.f;
			PlayerController->showLocation = MonsterLocation;
		}
	}
}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	PlayerInputComponent->BindAxis("MousePitch", this,&APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MouseYaw", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookupRate", this, &AMain::LookupAtRate);

	PlayerInputComponent->BindAction("Jumping", IE_Pressed, this, &AMain::Jump);
	PlayerInputComponent->BindAction("Jumping", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Quicken", IE_Pressed, this, &AMain::BeginQuicken);
	PlayerInputComponent->BindAction("Quicken", IE_Released, this, &AMain::EndQuicken);

	PlayerInputComponent->BindAction("Pick up/drop weapon", IE_Pressed, this, &AMain::PickUpWeapon);
	PlayerInputComponent->BindAction("Pick up/drop weapon", IE_Released, this, &AMain::DropWeapon);

	PlayerInputComponent->BindAction("attack", IE_Pressed, this, &AMain::AttackBegin);
	PlayerInputComponent->BindAction("attack", IE_Released, this, &AMain::AttackEnd);

	PlayerInputComponent->BindAction("ESC", IE_Pressed, this, &AMain::ShowPauseWidget);
	PlayerInputComponent->BindAction("ESC", IE_Released, this, &AMain::QuitPauseWidget);

	PlayerInputComponent->BindAction("SendLeven", IE_Pressed, this, &AMain::GotoNextLeven);
	PlayerInputComponent->BindAction("SendLeven", IE_Released, this, &AMain::_GotoNextLeven);
}

void AMain::MoveForward(float input) {

	if (!CanMove()) return;

	if (Controller && input != 0.0f && (!bAttacking)) {
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		MovementStatus = EMovementStatus::EMS_RUNING;

		//��������������Ƕ���x������
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, input);
	}
}

void AMain::MoveRight(float input) {
	if (!CanMove()) return;

	if (Controller && input != 0.0f && (!bAttacking)) {
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		MovementStatus = EMovementStatus::EMS_RUNING;

		//��������������Ƕ���x������
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, input);
	}
}

void AMain::Jump()
{
	if (CanMove()) {
		ACharacter::Jump();
	}
}

void AMain::TurnAtRate(float input)
{
	AddControllerYawInput(input * TurnRate * GetWorld()->GetDeltaSeconds());
}

void AMain::LookupAtRate(float input)
{
	AddControllerPitchInput(input * LookupRate * GetWorld()->GetDeltaSeconds());
}

/* *���� */
void AMain::BeginQuicken() {
	if (MovementStatus == EMovementStatus::EMS_Death || MovementStatus == EMovementStatus::EMS_Repel) return;
	is_quick = true;
}


void AMain::EndQuicken() {
	is_quick = false;
}



/* *����ը���յ��˺� */
void AMain::HpReduce(float num)
{
	if(num - Defense >= 0.f) this->CurrentHp -= (num - Defense);
	if (this->CurrentHp <= 0.f) {
		this->AIControllerClass = NULL;
		died();
	}
}

void AMain::RepelEnd() {
	if (MovementStatus == EMovementStatus::EMS_Repel) {
		SetMovementState(EMovementStatus::EMS_LDLE);
	}
}

/* *���� */
void AMain::died()
{
	if (MovementStatus == EMovementStatus::EMS_Death) return;
	MovementStatus = EMovementStatus::EMS_Death;

	/* *��ȡ��̫���ʵ�� */
	UAnimInstance* AnimInstance = this->GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage) {
		AnimInstance->Montage_Play(CombatMontage, 1.3f);
		AnimInstance->Montage_JumpToSection("death", CombatMontage);
	}
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
}

void AMain::EndDeath() {
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	float DeathTime = 3.f;
	//GetWorldTimerManager().SetTimer(DeathHandle, this, &AMain::DestroyActor, DeathTime);
}

void AMain::DestroyActor() {
	Destroy();
}

void AMain::IncreaseCoins(float Coinnum)
{
	this->cntCoins += Coinnum;
}


/* *���㿪ʼ������������������ */
void AMain::EpReduce(float num)
{
	this->CurrentEp -= num;
}

void AMain::EpRecovery(float num)
{
	if(CurrentEp < 100.f) this->CurrentEp += num;
}

void AMain::PickUpWeapon() {

	bisEquip = true;
	/* *ʹ�õ��� AItem ������ Weapon��ԭ����ǣ�Ϊ��Ϊ�����Ӹ����װ���������õ��Ƿ��࣬�����Ǳ��� */
	if (Overlapitem) {
		AWeapon* Weapon = Cast<AWeapon>(Overlapitem);
		if (Weapon && Weapon->WeaponStatus == EWeaponStatus::EWS_Ldle) {
			Weapon->equipWeapon(this);
			SetItemOverlap(nullptr);
		}
	}
}
void AMain::DropWeapon() {
	bisEquip = false;
}

void AMain::AttackBegin() {
	if (equipWeapon && !bAttacking && CanMove()) {
		Attack();
	}
}

void AMain::AttackEnd()
{
	if (MovementStatus == EMovementStatus::EMS_Death || MovementStatus == EMovementStatus::EMS_Repel) {
		bAttacking = false;
		SetInsterToMonster(false);

		/* repair bug Destroy weapon */
		SetItemOverlap(nullptr);
		return;
	}

	//bAttacking = false;
	UAnimInstance* AnimInstance = this->GetMesh()->GetAnimInstance();

	if (AnimInstance->Montage_GetIsStopped(CombatMontage)) {
		bAttacking = false;
		SetInsterToMonster(false);

		/* repair bug Destroy weapon */
		SetItemOverlap(nullptr);
	}
}

void AMain::Attack()
{
	if (!CanMove()) return;

	bAttacking = true;
	SetInsterToMonster(true);
	/* *��ȡ��̫���ʵ�� */
	UAnimInstance* AnimInstance = this->GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage) {
		int32 Section = FMath::RandRange(1, 2);
		
		switch (Section)
		{

		case 1:
			AnimInstance->Montage_Play(CombatMontage, 1.2f);
			AnimInstance->Montage_JumpToSection(FName("attack1"), CombatMontage);
			break;
		case 2:
			AnimInstance->Montage_Play(CombatMontage, 1.2f);
			AnimInstance->Montage_JumpToSection(FName("attack2"), CombatMontage);
			break;

		default:
			break;
		}
	}

}


void AMain::SetWeapon(AWeapon* SetWeapon)
{
	if (equipWeapon) {
		equipWeapon->Destroy();
	}
	equipWeapon = SetWeapon;
}

void AMain::SetInsterToMonster(bool SetInster)
{
	bInsterToMonster = SetInster;
}

FRotator AMain::GetYawToMonster(FVector LocationToMonster)
{
	FRotator YawToMonster = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LocationToMonster);
	return FRotator(0.f, YawToMonster.Yaw, 0.f);
}

float AMain::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && CombatMontage) {
		animInstance->Montage_Play(CombatMontage, 1.3f);
		animInstance->Montage_JumpToSection(FName("repel"), CombatMontage);
	}

	HpReduce(DamageTaken);

	return DamageTaken;
}

void AMain::SaveGame() {
	USaveMyGame* SaveGame =  Cast<USaveMyGame>(UGameplayStatics::CreateSaveGameObject(USaveMyGame::StaticClass()));
	SaveGame->CharacterState.CurHp = CurrentHp;
	SaveGame->CharacterState.MaxHp = MaxHp;
	SaveGame->CharacterState.CurEp = CurrentEp;
	SaveGame->CharacterState.MaxEp = MaxEp;
	SaveGame->CharacterState.CoinCnt = cntCoins;

	if (equipWeapon != nullptr) {
		SaveGame->CharacterState.SName = equipWeapon->SaveName;
	}

	SaveGame->CharacterState.Location = GetActorLocation();
	SaveGame->CharacterState.Rotation = GetActorRotation();

	UGameplayStatics::SaveGameToSlot(SaveGame, SaveGame->GameName, SaveGame->PlayerIndex);

}

void AMain::LoadGame(bool bLoad) {
	USaveMyGame* LoadGame = Cast<USaveMyGame>(UGameplayStatics::CreateSaveGameObject(USaveMyGame::StaticClass()));
	USaveMyGame* LoadGameInstance = Cast<USaveMyGame>(UGameplayStatics::LoadGameFromSlot(LoadGame->GameName, LoadGame->PlayerIndex));
	if (LoadGameInstance == nullptr) return;

	CurrentHp = LoadGameInstance->CharacterState.CurHp;
	MaxHp = LoadGameInstance->CharacterState.MaxHp;
	CurrentEp = LoadGameInstance->CharacterState.CurEp;
	MaxEp = LoadGameInstance->CharacterState.MaxEp;
	cntCoins = LoadGameInstance->CharacterState.CoinCnt;

	if (SaveWeapon) {
		/* *����AActor ��Ҫ������������. */
		AActorStorage* WeaponStorage = GetWorld()->SpawnActor<AActorStorage>(SaveWeapon);
		if (WeaponStorage) {
			FString LoadName = LoadGameInstance->CharacterState.SName;
			if (WeaponStorage->WeaponMap.Contains(LoadName)) {
				AWeapon* LoadWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponStorage->WeaponMap[LoadName]);
				LoadWeapon->equipWeapon(this);
			}
		}
	}

	if (bLoad) {
		SetActorLocation(LoadGameInstance->CharacterState.Location);
		SetActorRotation(LoadGameInstance->CharacterState.Rotation);
	}

	SetMovementState(EMovementStatus::EMS_LDLE);
	GetMesh()->bPauseAnims = false;
	GetMesh()->bNoSkeletonUpdate = false;
}


void AMain::ShowPauseWidget() {
	FilpBool(bShowEsc);
	if (bShowEsc) {
		if (PlayerController) {
			PlayerController->TogglePauseWidget();
		}
	}
	else {
		if (PlayerController) {
			PlayerController->TogglePauseWidget();
		}
	}
}

void AMain::QuitPauseWidget() {

}

void AMain::GotoNextLeven()
{
	if (Leven) {
		Leven->bSwitchLeven = true;
	}
}

void AMain::_GotoNextLeven()
{
	if (Leven) {
		if (Leven->targetPlayer) {
			Leven->targetPlayer = false;
		}
	}
}

void AMain::FilpBool(bool& b) {
	if (b == true) {
		b = false;
	}
	else {
		b = true;
	}
}

bool AMain::CanMove() {
	if (PlayerController) {
		return (MovementStatus != EMovementStatus::EMS_Death) &&
			(MovementStatus != EMovementStatus::EMS_Repel) && (!bAttacking) &&
			(!PlayerController->bshowPauseBar) && PlayerController->bCanMove;
	} 
	else {
		return false;
	}
}

void AMain::SwitchLeven(FName nextLeven)
{
	UWorld* world = GetWorld();
	FString curName = world->GetMapName();
	FName CurrentName(curName);
	if (CurrentName != nextLeven) {
		UGameplayStatics::OpenLevel(world, nextLeven);
	}
}
