// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchSystemBeam.h"

#include "Characters/BeamCharacter.h"

int UMatchSystemBeam::GetManche()
{
	return Manche;
}

void UMatchSystemBeam::SetManche(int NewManche)
{
	Manche = NewManche;
}

void UMatchSystemBeam::AddManche()
{
	Manche++;
}

void UMatchSystemBeam::SetMaxManche(int NewMaxManche)
{
	MaxManche = NewMaxManche;
}

int UMatchSystemBeam::GetMaxManche()
{
	return MaxManche;
}

int UMatchSystemBeam::GetPlayerPoints(int PlayerIndex)
{
	return PlayerPoints[PlayerIndex];
}

void UMatchSystemBeam::SetPlayerPoints(int PlayerIndex, int NewPoints)
{
	PlayerPoints[PlayerIndex] = NewPoints;
}

void UMatchSystemBeam::AddPlayerPoints(int PlayerIndex, int Points)
{
	PlayerPoints[PlayerIndex] += Points;
}

void UMatchSystemBeam::ResetPlayerPoints()
{
	for (int i = 0; i < PlayerPoints.Num(); i++)
	{
		PlayerPoints[i] = 0;
	}
}

void UMatchSystemBeam::SetPlayerPoints(TArray<int> NewPoints)
{
	PlayerPoints = NewPoints;
}

TArray<int> UMatchSystemBeam::GetPlayersPoints()
{
	return PlayerPoints;
}

int UMatchSystemBeam::GetMaxPoints()
{
	return MaxManche;
}

void UMatchSystemBeam::ChangePointWin()
{
	if (Characters.Num() == 0) return;

	if (!IsMancheFinished()) return;

	for (int i = 0; i < Characters.Num(); i++)
	{
		if (!Characters[i]->IsDead())
		{
			AddPlayerPoints(i, 1);
			LastIndexPlayerWin = i;
		}
	}
}

EMatchTypeID UMatchSystemBeam::GetMatchType()
{
	return MatchType;
}

void UMatchSystemBeam::SetMatchType(EMatchTypeID NewMatchType)
{
	MatchType = NewMatchType;
}

void UMatchSystemBeam::InitializeMatch(int numPlayers)
{
	if (Manche != 0) return;

	PlayerPoints.Empty();
	for (int i = 0; i < numPlayers; i++)
	{
		PlayerPoints.Add(0);
	}
}


bool UMatchSystemBeam::IsMatchFinished()
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

void UMatchSystemBeam::ResetAll()
{
	Manche = 0;
	PlayerPoints.Empty();
	Characters.Empty();
	MatchType = EMatchTypeID::Deathmatch;
}

void UMatchSystemBeam::SetCharacters(TArray<ABeamCharacter*> NewCharacters)
{
	Characters = NewCharacters;
}

void UMatchSystemBeam::CheckMatch()
{
}

int UMatchSystemBeam::GetLastIndexPlayerWin() {
	return LastIndexPlayerWin;
}

bool UMatchSystemBeam::IsMancheFinished()
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