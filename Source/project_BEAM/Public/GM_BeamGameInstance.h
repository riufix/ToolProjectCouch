// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MatchTypeID.h"
//#include <Match/BeamMatchSystem.h>
#include "MatchSystemBeam.h"

#include "GM_BeamGameInstance.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangePoints);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginLoading);

UCLASS(BlueprintType, Blueprintable)
class PROJECT_BEAM_API UGM_BeamGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	void Init() override;

	UPROPERTY(BlueprintAssignable)
	FOnChangePoints OnChangePoints;
	UPROPERTY(BlueprintAssignable)
	FOnBeginLoading OnBeginLoading;

	UFUNCTION(BlueprintCallable)
	void DeployEvent();

	UFUNCTION(BlueprintCallable)
	void SetLastSpawnNumber(int NewSpawnNumber);

	UFUNCTION(BlueprintCallable)
	int GetLastSpawnNumber();

	UFUNCTION(BlueprintCallable)
	void SetNumberPairAppeared(int NewNumber);

	UFUNCTION(BlueprintCallable)
	int GetNumberPairAppeared();

	UFUNCTION(BlueprintCallable)
	void ChangeMatchType(EMatchTypeID NewMatchType);

	UFUNCTION(BlueprintCallable)
	TArray<int> GetPlayersPoints() const;

	UFUNCTION(BlueprintCallable)
	UMatchSystemBeam* GetMancheSystem() const { return matchSystem; };

	UFUNCTION(BlueprintCallable)
	bool IsMatchFinished() {return matchSystem->IsMatchFinished();}

	UFUNCTION(BlueprintCallable)
	void ResetAll() {return matchSystem->ResetAll();}

	UFUNCTION(BlueprintCallable)
	void SetControllerToSkin(int playerController, int SkinIndex);
	UFUNCTION(BlueprintCallable)
	int GetControllerToSkin(int playerController);

protected:
	UPROPERTY()
	TMap<int, int> controllerToSkin;
	
	UPROPERTY(Blueprintable)
	TObjectPtr<UMatchSystemBeam> matchSystem;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameInstance")
	void TestBP();

private:
	UPROPERTY()
	int LastSpawnNumber = 0;
	UPROPERTY()
	int NumberPairAppeared = 0;

};