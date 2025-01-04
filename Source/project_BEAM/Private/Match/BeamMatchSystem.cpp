// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/BeamMatchSystem.h"

#include "Characters/BeamCharacter.h"

BeamMatchSystem::BeamMatchSystem()
{
}

BeamMatchSystem::~BeamMatchSystem()
{
}

int BeamMatchSystem::GetManche()
{
	return Manche;
}

void BeamMatchSystem::SetManche(int NewManche)
{
	Manche = NewManche;
}

void BeamMatchSystem::AddManche()
{
	Manche++;
}

void BeamMatchSystem::SetMaxManche(int NewMaxManche)
{
	MaxManche = NewMaxManche;
}

int BeamMatchSystem::GetMaxManche()
{
	return MaxManche;
}

int BeamMatchSystem::GetPlayerPoints(int PlayerIndex)
{
	return PlayerPoints[PlayerIndex];
}

void BeamMatchSystem::SetPlayerPoints(int PlayerIndex, int NewPoints)
{
	PlayerPoints[PlayerIndex] = NewPoints;
}

void BeamMatchSystem::AddPlayerPoints(int PlayerIndex, int Points)
{
	PlayerPoints[PlayerIndex] += Points;
}

void BeamMatchSystem::ResetPlayerPoints()
{
	for (int i = 0; i < PlayerPoints.Num(); i++)
	{
		PlayerPoints[i] = 0;
	}
}

void BeamMatchSystem::SetPlayerPoints(TArray<int> NewPoints)
{
	PlayerPoints = NewPoints;
}

TArray<int> BeamMatchSystem::GetPlayersPoints()
{
	return PlayerPoints;
}

int BeamMatchSystem::GetMaxPoints()
{
	return MaxManche;
}

void BeamMatchSystem::ChangePointWin()
{
	if (Characters.Num() == 0) return;

	if (!IsMancheFinished()) return;

	for (int i = 0; i < Characters.Num(); i++)
	{
		if (!Characters[i]->IsDead())
		{
			AddPlayerPoints(i, 1);
		}
	}
}

EMatchTypeID BeamMatchSystem::GetMatchType()
{
	return MatchType;
}

void BeamMatchSystem::SetMatchType(EMatchTypeID NewMatchType)
{
	MatchType = NewMatchType;
}

void BeamMatchSystem::InitializeMatch(int numPlayers)
{
	if (Manche != 0) return;

	PlayerPoints.Empty();
	for (int i = 0; i < numPlayers; i++)
	{
		PlayerPoints.Add(0);
	}
}


bool BeamMatchSystem::IsMatchFinished()
{
	for (int i = 0; i < PlayerPoints.Num(); i++)
	{
		if (PlayerPoints[i] >= MaxManche)
		{
			return true;
		}
	}

	return false;
}

void BeamMatchSystem::ResetAll()
{
	Manche = 0;
	PlayerPoints.Empty();
	Characters.Empty();
	MatchType = EMatchTypeID::Deathmatch;
}

void BeamMatchSystem::SetCharacters(TArray<ABeamCharacter*> NewCharacters)
{
	Characters = NewCharacters;
}

void BeamMatchSystem::CheckMatch()
{
}

bool BeamMatchSystem::IsMancheFinished()
{
	int PlayersAlive = 0;
	int size = Characters.Num();

	for (int i = 0; i < size; i++)
	{
		if (!Characters[i]->IsDead())
		{
			PlayersAlive++;
		}
	}

	if (PlayersAlive <= 1)
	{
		return true;
	}
	else {
		return false;
	}
}