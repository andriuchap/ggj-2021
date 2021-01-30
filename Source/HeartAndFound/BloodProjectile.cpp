// Fill out your copyright notice in the Description page of Project Settings.


#include "BloodProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABloodProjectile::ABloodProjectile(const FObjectInitializer &ObjInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;
	CollisionSphere->SetSphereRadius(32.0F);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = RootComponent;
}

// Called when the game starts or when spawned
void ABloodProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABloodProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}