// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_BeamGameInstance.h"
#include "MatchSystemBeam.h"

#include "Match/BeamMatchSystem.h"

void UGM_BeamGameInstance::Init()
{
	Super::Init();
	matchSystem = NewObject<UMatchSystemBeam>(this);
	controllerToSkin.Add(0, 0);
	controllerToSkin.Add(1, 1);
}

void UGM_BeamGameInstance::DeployEvent()
{
	OnChangePoints.Broadcast();
}


void UGM_BeamGameInstance::SetLastSpawnNumber(int NewSpawnNumber)
{
	LastSpawnNumber = NewSpawnNumber;
}

int UGM_BeamGameInstance::GetLastSpawnNumber()
{
	return LastSpawnNumber;
}

void UGM_BeamGameInstance::SetNumberPairAppeared(int NewNumber)
{
	NumberPairAppeared = NewNumber;
}

int UGM_BeamGameInstance::GetNumberPairAppeared()
{
	return NumberPairAppeared;
}

void UGM_BeamGameInstance::ChangeMatchType(EMatchTypeID NewMatchType)
{
	matchSystem->SetMatchType(NewMatchType);
}

TArray<int> UGM_BeamGameInstance::GetPlayersPoints() const
{
	return matchSystem->GetPlayersPoints();
}

void UGM_BeamGameInstance::SetControllerToSkin(int playerController, int SkinIndex)
{
	if (controllerToSkin.Contains(playerController))
		controllerToSkin.Emplace(playerController, SkinIndex);
	else controllerToSkin.Add(playerController, SkinIndex);
}
int UGM_BeamGameInstance::GetControllerToSkin(int playerController)
{
	TestBP();
	OnBeginLoading.Broadcast();
	if (!controllerToSkin.Contains(playerController)) return -1;
	return *controllerToSkin.Find(playerController);
}

void UGM_BeamGameInstance::TestBP_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("C++ Default Implementation"));

	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnTestBP Blueprint event should be executed now!"));
	}
}
