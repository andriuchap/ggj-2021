// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BloodProjectile.generated.h"

UCLASS()
class HEARTANDFOUND_API ABloodProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABloodProjectile(const FObjectInitializer &ObjInitializer);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Blood")
	class USphereComponent* CollisionSphere;

	UPROPERTY(VisibleDefaultsOnly, Category = "Blood")
	class UProjectileMovementComponent* ProjectileMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE USphereComponent* GetCollisionSphere() { return CollisionSphere; }
	FORCEINLINE UProjectileMovementComponent* GetMovementComp() { return ProjectileMovement; }
};
