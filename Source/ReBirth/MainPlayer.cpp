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

	//��ײ���
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->SetSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	//mesh ���
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	MeshComponent->SetupAttachment(GetRootComponent());

	//���ɱ����
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bEnableCameraLag = true; // �����ӳ�
	SpringArm->CameraLagSpeed = 3.0f; // ����ӳ�ʱ��

	//������
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	CameraInput = FVector2D(0.f, 0.f);

	//�ƶ���� 
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

	//ע�⣺ Ҫ��SetActorRotationִ�н����ڽ��� pringarm����ת�ǶȻ�ȡ��
	FRotator NewPringArmRotation = SpringArm->GetComponentRotation();
	NewPringArmRotation.Pitch = FMath::Clamp(NewPringArmRotation.Pitch + CameraInput.Y, -80.0f, -15.0f);
	SpringArm->SetWorldRotation(NewPringArmRotation);
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	// ������ҵ��ƶ�
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainPlayer::MoveRight);

	//�����ת
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

