// Fill out your copyright notice in the Description page of Project Settings.


#include "TemperatureVolumeActor.h"
#include "Components/BoxComponent.h"
#include "HeartAndFoundCharacter.h"

// Sets default values
ATemperatureVolumeActor::ATemperatureVolumeActor(const FObjectInitializer &ObjInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionProfileName(FName("OverlapOnlyPawn"));
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATemperatureVolumeActor::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATemperatureVolumeActor::OnBoxEndOverlap);

	TemperatureChangeRate = 10.0F;
	bLimitAmount = false;
	TemperatureAmount = 0.0F;

	AffectedCharacter = nullptr;
}

// Called when the game starts or when spawned
void ATemperatureVolumeActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

void ATemperatureVolumeActor::OnBoxBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AHeartAndFoundCharacter* Character = Cast<AHeartAndFoundCharacter>(OtherActor);
	if (Character != nullptr)
	{
		if (!bLimitAmount || TemperatureAmount > 0.0F)
		{
			AffectedCharacter = Character;
			SetActorTickEnabled(true);
		}
	}
}

void ATemperatureVolumeActor::OnBoxEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	AHeartAndFoundCharacter* Character = Cast<AHeartAndFoundCharacter>(OtherActor);
	if (Character != nullptr)
	{
		if (AffectedCharacter != nullptr)
		{
			AffectedCharacter = nullptr;
			SetActorTickEnabled(false);
		}
	}
}

void ATemperatureVolumeActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (AffectedCharacter != nullptr)
	{
		float Change = TemperatureChangeRate * DeltaSeconds;
		if (bLimitAmount && TemperatureAmount > 0.0F)
		{
			TemperatureAmount -= FMath::Abs(Change);
			AffectedCharacter->ChangeTemperature(Change);
			if (TemperatureAmount <= 0.0F)
			{
				SourceExtinguished();
			}
		}
		else if (!bLimitAmount)
		{
			AffectedCharacter->ChangeTemperature(Change);
		}
	}
}
