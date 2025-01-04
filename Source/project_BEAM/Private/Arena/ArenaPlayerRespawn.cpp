// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena/ArenaPlayerRespawn.h"


float AArenaPlayerRespawn::GetStartOrientX() const
{
	switch (StartOrient)
	{
	case EArenaCharacterOrient::Left: return -1.f;
	case EArenaCharacterOrient::Right: return 1.f;
	}

	return 0.f;
}

