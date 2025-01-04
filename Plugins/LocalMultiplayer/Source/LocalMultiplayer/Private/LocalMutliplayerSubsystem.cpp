// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMutliplayerSubsystem.h"
#include "LocalMultiplayerSettings.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"

void ULocalMultiplayerSubsystem::CreateAndInitPlayers()
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	int NumberOfControllers = LocalMultiplayerSettings->NbMaxGamepads;
	
	//Création des PC gamepad
	for (int i = 0; i < NumberOfControllers; i++)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Purple,
			FString::Printf(TEXT("INIT PLAYER : %d"), i)
		);
		UGameplayStatics::CreatePlayer(GetWorld(),-1);
	}
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID)
{
	if(PlayerIndexFromGamepadProfileIndex.Contains(DeviceID))
	{
		return PlayerIndexFromGamepadProfileIndex[DeviceID];
	}
	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID)
{
	LastAssignedPlayerIndex++;
	PlayerIndexFromGamepadProfileIndex.Add(DeviceID, LastAssignedPlayerIndex);
	
	return LastAssignedPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex) const
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();

	APlayerController* LocalPlayer = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex);
	UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = LocalPlayer->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if(PlayerSubsystem == nullptr) return;
	
	UInputMappingContext* IMC = LocalMultiplayerSettings->GamepadProfileData.IMCGames;
	if(IMC == nullptr) return;
	
	FModifyContextOptions FModify;
	FModify.bForceImmediately = true;
	PlayerSubsystem->AddMappingContext(IMC, 1, FModify);
}