// Copyright Epic Games, Inc. All Rights Reserved.

#include "RealTestMultiplayerGameMode.h"
#include "RealTestMultiplayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARealTestMultiplayerGameMode::ARealTestMultiplayerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
