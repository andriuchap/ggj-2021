// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HeartAndFoundCharacter.generated.h"

UCLASS(config=Game)
class AHeartAndFoundCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AHeartAndFoundCharacter(const FObjectInitializer& ObjInitializer);

protected:
	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, Category = Camera)
		FVector CameraOffsetWhenMoving;

	UPROPERTY(EditDefaultsOnly, Category = "Heart")
	float MaxTemperature;

	UPROPERTY(EditDefaultsOnly, Category = "Heart")
	float DefaultDrainRate;

	UPROPERTY(EditDefaultsOnly, Category = "Heart")
	float JumpTemperatureCost;

	UPROPERTY(EditDefaultsOnly, Category = "Heart")
	int MaxBloodAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Heart")
		float ThrowCooldown;

	UPROPERTY(EditDefaultsOnly, Category = "Heart")
	TSubclassOf<class ABloodProjectile> BloodProjectileClass;

private:
	float CurrentTemperature;

	float BaseArmLength;

	int BloodAmmo;
	bool bCanThrowBlood;
	FTimerHandle TimerHandle_BloodThrow;

public:
	void ChangeTemperature(float InAmount);

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	void ThrowBlood();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;
public:

	virtual void Tick(float DeltaSeconds) override;

	virtual void Jump() override;

	void AllowThrow();
	UFUNCTION(BlueprintCallable, Category = "Heart")
	void AddAmmo(int InAmount);

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE bool IsDead() { return CurrentTemperature <= 0.0F; }

	UFUNCTION(BlueprintPure, Category = "Heart")
	FORCEINLINE float GetMaxTemperature() { return MaxTemperature; }

	UFUNCTION(BlueprintPure, Category = "Heart")
	FORCEINLINE float GetCurrentTemperature() { return CurrentTemperature; }

	UFUNCTION(BlueprintPure, Category = "Heart")
	FORCEINLINE float GetAmmoPercentage() { return (float)BloodAmmo / MaxBloodAmmo; }
};
