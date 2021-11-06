// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerMovementComponent.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	//碰撞组件
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->SetSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	//mesh 组件
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	MeshComponent->SetupAttachment(GetRootComponent());

	//弹簧臂组件
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bEnableCameraLag = true; // 允许延迟
	SpringArm->CameraLagSpeed = 3.0f; // 相机延迟时间

	//相机组件
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	CameraInput = FVector2D(0.f, 0.f);

	//移动组件 
	OurMovementComponent = CreateDefaultSubobject<UPlayerMovementComponent>(TEXT("OurMovement"));
	OurMovementComponent->UpdatedComponent = RootComponent;

}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += CameraInput.X;
	SetActorRotation(NewRotation);

	//注意： 要让SetActorRotation执行结束在进行 pringarm的旋转角度获取！
	FRotator NewPringArmRotation = SpringArm->GetComponentRotation();
	NewPringArmRotation.Pitch = FMath::Clamp(NewPringArmRotation.Pitch + CameraInput.Y, -80.0f, -15.0f);
	SpringArm->SetWorldRotation(NewPringArmRotation);
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	// 控制玩家的移动
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainPlayer::MoveRight);

	//相机旋转
	PlayerInputComponent->BindAxis(TEXT("MousePitch"), this, &AMainPlayer::CameraPitch);
	PlayerInputComponent->BindAxis(TEXT("MouseYaw"), this, &AMainPlayer::CameraYaw);
}

void AMainPlayer::MoveForward(float value)
{
	FVector ForwardVector = GetActorForwardVector();
	if (OurMovementComponent) {
		OurMovementComponent->AddInputVector(ForwardVector * value);
	}
}

void AMainPlayer::MoveRight(float value)
{
	FVector RightVector = GetActorRightVector();
	if (OurMovementComponent) {
		OurMovementComponent->AddInputVector(RightVector * value);
	}
}

void AMainPlayer::CameraPitch(float value) {
	CameraInput.Y = value;
}

void AMainPlayer::CameraYaw(float value) {
	CameraInput.X = value;
}

UPawnMovementComponent* AMainPlayer::GetMovementComponent() const
{
	return OurMovementComponent;
}

