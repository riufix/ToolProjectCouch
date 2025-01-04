// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaCharacterOrient.h"
#include "GameFramework/PlayerStart.h"
#include "ArenaPlayerRespawn.generated.h"

UCLASS()
class PROJECT_BEAM_API AArenaPlayerRespawn : public APlayerStart
{
	GENERATED_BODY()

public:
	UFUNCTION()
	float GetStartOrientX() const;

	UPROPERTY(EditAnywhere)
	EArenaCharacterOrient StartOrient = EArenaCharacterOrient::Right;
};