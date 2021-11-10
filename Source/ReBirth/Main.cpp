// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	/* Init Enums */
	MovementStatus = EMovementStatus::EMS_WALK;
	EpStatus = EEpStatus::EES_Normal;

	/*��Ep С��ExhaustLimite ����MinmumLimiteΪ��ɫ����EpС�� MinmumLimiteʱΪ��ɫ*/
	ExhaustLimite = 0.6f;
	MinmumLimite = 0.25f;

	//��ȡ��ҿ�����
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	if (is_quick && CurrentEp > 0.f) {
		/* Player pressed left Shift add Walk speed... */
		float speed = this->GetCharacterMovement()->GetMaxSpeed();
		this->GetCharacterMovement()->MaxWalkSpeed = (speed + 10.0f <= 330) ? speed + 10.0f : speed;
		EpReduce(0.7);

		if ((CurrentEp / MaxEp) <= 0.6f && (CurrentEp / MaxEp) > 0.25f) SetEpStatus(EEpStatus::EES_Exhaust);
		else if ((CurrentEp / MaxEp) <= 0.25f) SetEpStatus(EEpStatus::EES_Minmum);
		else SetEpStatus(EEpStatus::EES_Normal);

	}
	else {
		/* ...Released the Walk speed updata 230.0f */
		this->GetCharacterMovement()->MaxWalkSpeed = 230.0f;
		EpRecovery(0.2);

		if ((CurrentEp / MaxEp) >= 0.25f && (CurrentEp / MaxEp) < 0.6f) SetEpStatus(EEpStatus::EES_Exhaust);
		else if ((CurrentEp / MaxEp) >=  0.6f) SetEpStatus(EEpStatus::EES_Normal);
		else SetEpStatus(EEpStatus::EES_Minmum);
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

	PlayerInputComponent->BindAction("Jumping", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jumping", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Quicken", IE_Pressed, this, &AMain::BeginQuicken);
	PlayerInputComponent->BindAction("Quicken", IE_Released, this, &AMain::EndQuicken);
}

void AMain::MoveForward(float input) {
	if (Controller && input != 0.0f) {
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		//��������������Ƕ���x������
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, input);
	}
}

void AMain::MoveRight(float input) {
	if (Controller && input != 0.0f) {
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		//��������������Ƕ���x������
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, input);
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
	is_quick = true;
}


void AMain::EndQuicken() {
	is_quick = false;
}


/* *����ը���յ��˺� */
void AMain::HpReduce(float num)
{
	this->CurrentHp -= num;
	if (this->CurrentHp <= 0.f) {
		died();
	}
}

/* *���� */
void AMain::died()
{

}

void AMain::IncreaseCoins()
{
	this->cntCoins++;
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