// Copyright Epic Games, Inc. All Rights Reserved.

#include "project_BEAMGameMode.h"
#include "project_BEAMCharacter.h"
#include "UObject/ConstructorHelpers.h"

Aproject_BEAMGameMode::Aproject_BEAMGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
