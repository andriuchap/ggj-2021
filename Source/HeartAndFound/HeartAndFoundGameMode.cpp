// Copyright Epic Games, Inc. All Rights Reserved.

#include "HeartAndFoundGameMode.h"
#include "HeartAndFoundCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHeartAndFoundGameMode::AHeartAndFoundGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
