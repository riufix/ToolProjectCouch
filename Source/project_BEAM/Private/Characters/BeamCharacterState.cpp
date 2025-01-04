// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BeamCharacterState.h"
#include "Characters/BeamCharacterStateMachine.h"

#include "Characters/BeamCharacter.h"

// Sets default values for this component's properties
UBeamCharacterState::UBeamCharacterState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

EBeamCharacterStateID UBeamCharacterState::GetStateID()
{
	return EBeamCharacterStateID::None;
}

void UBeamCharacterState::StateInit(UBeamCharacterStateMachine* InStateMachine)
{
	StateMachine = InStateMachine;
	Character = InStateMachine->GetCharacter();

	GEngine->AddOnScreenDebugMessage(
		-1, 
		5.f, 
		FColor::Red, 
		FString::Printf(TEXT("Init State %d"), GetStateID()));


}

void UBeamCharacterState::StateEnter(EBeamCharacterStateID PreviousStateID)
{
}

void UBeamCharacterState::StateExit(EBeamCharacterStateID NextStateID)
{
}

void UBeamCharacterState::StateTick(float DeltaTime)
{
	if (IsKeyWasPressed(EKeys::U) && Character) {
		Character->PlayerTakeDamage(3);
		
		GEngine->AddOnScreenDebugMessage(
			-1,
			0.1f,
			FColor::Blue,
			FString::Printf(TEXT("Debug Take Damage"))
		);
	}
}

bool const UBeamCharacterState::IsKeyDown(FKey key) const {
	if (Character == nullptr) return false;
	if(Character->GetLocalViewingPlayerController() == nullptr) return false;
	
	return Character->GetLocalViewingPlayerController()->IsInputKeyDown(key);
}

bool const UBeamCharacterState::IsKeyWasPressed(FKey key) const
{
	if (Character == nullptr) return false;
	if(Character->GetLocalViewingPlayerController() == nullptr) return false;
	
	return Character->GetLocalViewingPlayerController()->WasInputKeyJustPressed(key);
}

void UBeamCharacterState::RedoParams()
{
}