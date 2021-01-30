// Copyright Epic Games, Inc. All Rights Reserved.

#include "HeartAndFoundCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BloodProjectile.h"
#include "TimerManager.h"
#include "Engine/World.h"

AHeartAndFoundCharacter::AHeartAndFoundCharacter(const FObjectInitializer& ObjInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));
	CameraBoom->bEnableCameraLag = true;

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	CameraOffsetWhenMoving = FVector(0.0F, 275.0F, 275.0F);

	MaxTemperature = 100.0F;
	CurrentTemperature = MaxTemperature;

	DefaultDrainRate = 1.0F;

	JumpTemperatureCost = 5.0F;

	MaxBloodAmmo = 100;
	BloodAmmo = 0;

	ThrowCooldown = 0.5F;
	bCanThrowBlood = true;

	BloodProjectileClass = ABloodProjectile::StaticClass();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AHeartAndFoundCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AHeartAndFoundCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHeartAndFoundCharacter::MoveRight);
}

void AHeartAndFoundCharacter::BeginPlay()
{
	Super::BeginPlay();

	BaseArmLength = CameraBoom->TargetArmLength;
}

void AHeartAndFoundCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ChangeTemperature(-DefaultDrainRate * DeltaSeconds);

	float MaxSpeed = GetCharacterMovement()->MaxWalkSpeed;
	FVector Velocity = GetCharacterMovement()->Velocity;
	float HParam = ((Velocity.Y / MaxSpeed) + 1) * 0.5F;
	float HCameraDistance = FMath::Lerp<float>(-CameraOffsetWhenMoving.Y, CameraOffsetWhenMoving.Y, HParam);
	CameraBoom->TargetOffset.Y = HCameraDistance;

	FVector CharLocation = GetActorLocation();
	float AdditionalArmLength = FMath::Lerp<float>(0.0F, 1800.0F, FMath::Clamp<float>((CharLocation.Z - 200.0F) / 800.0F, 0.0F, 1.0F));
	CameraBoom->TargetOffset.X = AdditionalArmLength;
}

void AHeartAndFoundCharacter::Jump()
{
	if (!IsDead())
	{
		Super::Jump();
		ChangeTemperature(-JumpTemperatureCost);
	}
}

void AHeartAndFoundCharacter::AllowThrow()
{
	bCanThrowBlood = true;
}

void AHeartAndFoundCharacter::AddAmmo(int InAmount)
{
	BloodAmmo += InAmount;
	if (BloodAmmo > MaxBloodAmmo)
	{
		BloodAmmo = MaxBloodAmmo;
	}
}

void AHeartAndFoundCharacter::ChangeTemperature(float InAmount)
{
	if (!IsDead())
	{
		CurrentTemperature = FMath::Clamp(InAmount + CurrentTemperature, 0.0F, MaxTemperature);
		if (CurrentTemperature == 0.0F)
		{
			StopJumping();
		}
	}
}

void AHeartAndFoundCharacter::MoveRight(float Value)
{
	// add movement in that direction
	if (!IsDead())
	{
		AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
	}
}

void AHeartAndFoundCharacter::ThrowBlood()
{
	if (bCanThrowBlood && BloodAmmo > 0)
	{
		bCanThrowBlood = false;
		BloodAmmo--;
		// Spawn projectile



		ABloodProjectile* BloodProj = GetWorld()->SpawnActor<ABloodProjectile>(BloodProjectileClass, );

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_BloodThrow, this, &AHeartAndFoundCharacter::AllowThrow, ThrowCooldown);
	}
}

