// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBeamCharacterStateID : uint8
{
	None = 0,
	Idle,
	Walk,
	Jump,
	Fall,
	Land,
	Push,
	Fly,
	Stun,
	Projection,
	Dead
};