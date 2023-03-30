// Copyright Epic Games, Inc. All Rights Reserved.

#include "MedivalDungeonGameMode.h"
#include "MedivalDungeonCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMedivalDungeonGameMode::AMedivalDungeonGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
