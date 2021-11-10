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
	CameraArm->bUsePawnControlRotation = true; // 希望弹簧杆可以跟着玩家运动


	// PlayerEyes
	PlayerEye = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerEye"));
	PlayerEye->SetupAttachment(CameraArm, USpringArmComponent::SocketName); // 我们需要在 弹簧杆上有一个插槽来插入一个 相机
	PlayerEye->bUsePawnControlRotation = false; // 我们不希望相机可以单独旋转，他只要跟着弹簧杆就好

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; //基于移动方向角色正面的朝向旋转...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f); //...只旋转Yaw轴（学到了！）
	GetCharacterMovement()->JumpZVelocity = 500.f; // 跳跃速度
	
	/*  *我认为在空中移动是不合理的, 所以我并没有添加
		*GetCharacterMovement()->AirControl= 0.4f; */


	/* 初始化属性 */
	MaxHp = 100.f;
	CurrentHp = 100.f;
	MaxEp = 100.f;
	CurrentEp = 100.f;
	cntCoins = 0.f;

	/* Init Enums */
	MovementStatus = EMovementStatus::EMS_WALK;
	EpStatus = EEpStatus::EES_Normal;

	/*当Ep 小于ExhaustLimite 大于MinmumLimite为黄色，当Ep小于 MinmumLimite时为红色*/
	ExhaustLimite = 0.6f;
	MinmumLimite = 0.25f;

	//获取玩家控制器
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

		//来获得相对于这个角度中x的向量
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, input);
	}
}

void AMain::MoveRight(float input) {
	if (Controller && input != 0.0f) {
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		//来获得相对于这个角度中x的向量
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

/* *加速 */
void AMain::BeginQuicken() {
	is_quick = true;
}


void AMain::EndQuicken() {
	is_quick = false;
}


/* *碰到炸弹收到伤害 */
void AMain::HpReduce(float num)
{
	this->CurrentHp -= num;
	if (this->CurrentHp <= 0.f) {
		died();
	}
}

/* *死亡 */
void AMain::died()
{

}

void AMain::IncreaseCoins()
{
	this->cntCoins++;
}


/* *当你开始奔跑你的体力将会减少 */
void AMain::EpReduce(float num)
{
	this->CurrentEp -= num;
}

void AMain::EpRecovery(float num)
{
	if(CurrentEp < 100.f) this->CurrentEp += num;
}