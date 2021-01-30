// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TemperatureVolumeActor.generated.h"

UCLASS()
class HEARTANDFOUND_API ATemperatureVolumeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATemperatureVolumeActor(const FObjectInitializer &ObjInitializer);

protected:

	UPROPERTY(VisibleDefaultsOnly, Category = "Cold")
		class UBoxComponent* BoxComponent;

	// How fast will the temperature decrease
	UPROPERTY(EditAnywhere, Category = "Cold")
		float TemperatureChangeRate;

	UPROPERTY(EditAnywhere, Category = "Cold")
		bool bLimitAmount;

	// How much temperature the area can decrease
	UPROPERTY(EditAnywhere, Category = "Cold")
		float TemperatureAmount;

private:

	class AHeartAndFoundCharacter* AffectedCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, Category = "Cold")
	void SourceExtinguished();

public:

	virtual void Tick(float DeltaSeconds) override;
};
