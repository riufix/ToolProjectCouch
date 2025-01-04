// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/BeamCharacterStateDeath.h"

EBeamCharacterStateID UBeamCharacterStateDeath::GetStateID()
{
	return EBeamCharacterStateID::Dead;
}

void UBeamCharacterStateDeath::StateEnter(EBeamCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
}

void UBeamCharacterStateDeath::StateExit(EBeamCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
}

void UBeamCharacterStateDeath::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}
