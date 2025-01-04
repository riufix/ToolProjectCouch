// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BeamCharacterStateMachine.h"

#include "Characters/BeamCharacterState.h"

#include "Characters/BeamCharacter.h"


void UBeamCharacterStateMachine::Init(ABeamCharacter* NewCharacter)
{
	Character = NewCharacter;
	FindStates();
	InitStates();

	ChangeState(EBeamCharacterStateID::Idle);
}

void UBeamCharacterStateMachine::Tick(float DeltaTime)
{
	if (CurrentState != nullptr) CurrentState->StateTick(DeltaTime);
}

ABeamCharacter* UBeamCharacterStateMachine::GetCharacter() const
{
	return Character;
}

void UBeamCharacterStateMachine::ChangeState(EBeamCharacterStateID NewStateID)
{
	if (!canChangeState) return;

	UBeamCharacterState* NewState = GetState(NewStateID);

	if (NewState == nullptr) return;

	if (CurrentState != nullptr) 
	{
		CurrentState->StateExit(NewStateID);
	}

	EBeamCharacterStateID PreviousStateID = CurrentStateID;
	CurrentStateID = NewStateID;
	CurrentState = NewState;

	if (CurrentState != nullptr)
	{
		CurrentState->StateEnter(PreviousStateID);
	}

}

UBeamCharacterState* UBeamCharacterStateMachine::GetState(EBeamCharacterStateID StateID)
{
	for (UBeamCharacterState* State : AllStates)
	{
		if (StateID == State->GetStateID())
		{
			return State;
		}
	}

	return nullptr;
}

EBeamCharacterStateID const UBeamCharacterStateMachine::GetCurrentStateID() const
{

	if (CurrentState == nullptr) return EBeamCharacterStateID::None;

	return CurrentState->GetStateID();
}

void UBeamCharacterStateMachine::RedoParams()
{
	if (CurrentState == nullptr) return;
	CurrentState->RedoParams();
}

void UBeamCharacterStateMachine::SetCanChangeState(bool canChange)
{
	canChangeState = canChange;
}

bool UBeamCharacterStateMachine::GetCanChangeState() const
{
	return canChangeState;
}

void UBeamCharacterStateMachine::FindStates()
{
	TArray<UActorComponent*> FoundComponents = Character->K2_GetComponentsByClass(UBeamCharacterState::StaticClass());
	for (UActorComponent* StateComponent : FoundComponents)
	{
		UBeamCharacterState* State = Cast<UBeamCharacterState>(StateComponent);
		if (State == nullptr) continue;
		if (State->GetStateID() == EBeamCharacterStateID::None) continue;

		AllStates.Add(State);

	}
}

void UBeamCharacterStateMachine::InitStates()
{
	for (UBeamCharacterState* State : AllStates)
	{
		State->StateInit(this);
	}
}
