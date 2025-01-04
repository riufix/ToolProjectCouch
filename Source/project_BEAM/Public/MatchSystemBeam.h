// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MatchTypeID.h"



#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MatchSystemBeam.generated.h"

class ABeamCharacter;

/**
 * 
 */
UCLASS()
class PROJECT_BEAM_API UMatchSystemBeam : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	int GetManche();

	UFUNCTION(BlueprintCallable)
	void SetManche(int NewManche);

	UFUNCTION(BlueprintCallable)
	void AddManche();

	UFUNCTION(BlueprintCallable)
	void SetMaxManche(int NewMaxManche);

	UFUNCTION(BlueprintCallable)
	int GetMaxManche();

	// MANCHE

	// POINTS

	UFUNCTION(BlueprintCallable)
	int GetPlayerPoints(int PlayerIndex);

	UFUNCTION(BlueprintCallable)
	void SetPlayerPoints(int PlayerIndex, int NewPoints);

	UFUNCTION(BlueprintCallable)
	void AddPlayerPoints(int PlayerIndex, int Points);

	UFUNCTION(BlueprintCallable)
	void ResetPlayerPoints();

	void SetPlayerPoints(TArray<int> NewPoints);

	UFUNCTION(BlueprintCallable)
	TArray<int> GetPlayersPoints();

	UFUNCTION(BlueprintCallable)
	int GetMaxPoints();

	UFUNCTION(BlueprintCallable)
	void ChangePointWin();

	// POINTS

	// MATCH

	UFUNCTION(BlueprintCallable)
	EMatchTypeID GetMatchType();

	UFUNCTION(BlueprintCallable)
	void SetMatchType(EMatchTypeID NewMatchType);

	UFUNCTION(BlueprintCallable)
	void InitializeMatch(int numPlayers);

	UFUNCTION(BlueprintCallable)
	bool IsMatchFinished();

	UFUNCTION(BlueprintCallable)
	void ResetAll();

	// MATCH

	UFUNCTION(BlueprintCallable)
	void SetCharacters(TArray<ABeamCharacter*> NewCharacters);

	UFUNCTION(BlueprintCallable)
	TArray<ABeamCharacter*> GetCharacters() { return Characters; }

	UFUNCTION(BlueprintCallable)
	void CheckMatch();

	UFUNCTION(BlueprintCallable)
	bool IsMancheFinished();

	UFUNCTION(BlueprintCallable)
	int GetLastIndexPlayerWin();

private:
	UPROPERTY()
	int Manche = 0;

	UPROPERTY()
	TArray<int> PlayerPoints = {};

	UPROPERTY()
	int MaxManche = 3;

	UPROPERTY()
	TArray<ABeamCharacter*> Characters = {};

	UPROPERTY()
	int LastIndexPlayerWin = 0;

	UPROPERTY()
	EMatchTypeID MatchType = EMatchTypeID::Deathmatch;

};
