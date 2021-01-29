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

	UPROPERTY(EditDefaultsOnly, Category = "Heart")
	float MaxTemperature;

	UPROPERTY(EditDefaultsOnly, Category = "Heart")
	float DefaultDrainRate;

private:
	float CurrentTemperature;

public:
	void ChangeTemperature(float InAmount);

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface


public:

	virtual void Tick(float DeltaSeconds) override;

	void AddTemperatureChangeRate(float InAmount);
	void RemoveTemperatureChangeRate(float InAmount);

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE bool IsDead() { return CurrentTemperature <= 0.0F; }

	UFUNCTION(BlueprintPure, Category = "Heart")
	FORCEINLINE float GetMaxTemperature() { return MaxTemperature; }

	UFUNCTION(BlueprintPure, Category = "Heart")
	FORCEINLINE float GetCurrentTemperature() { return CurrentTemperature; }
};
