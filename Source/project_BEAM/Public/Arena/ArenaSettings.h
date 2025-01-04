// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ArenaSettings.generated.h"

class ABeamCharacter;

UCLASS(Config=Game, DefaultConfig, meta = (DisplayName = "Beam Arena Settings"))
class PROJECT_BEAM_API UArenaSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Characters")
	TSubclassOf<ABeamCharacter> BeamCharacterClassP1;

	UPROPERTY(Config, EditAnywhere, Category="Characters")
	TSubclassOf<ABeamCharacter> BeamCharacterClassP2;

	UPROPERTY(Config, EditAnywhere, Category = "Manche")
	int MancheDiffShield = 2;
};
