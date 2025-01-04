// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/BeamCharacterStateID.h"


#include "BeamCharacterState.generated.h"

class ABeamCharacter;
class UBeamCharacterStateMachine;

UCLASS(Abstract)
class UBeamCharacterState : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBeamCharacterState();

	virtual EBeamCharacterStateID GetStateID();

	virtual void StateInit(UBeamCharacterStateMachine* InStateMachine);

	virtual void StateEnter(EBeamCharacterStateID PreviousStateID);

	virtual void StateExit(EBeamCharacterStateID NextStateID);

	virtual void StateTick(float DeltaTime);

	bool const IsKeyDown(FKey key) const;

	bool const IsKeyWasPressed(FKey key) const;	

	virtual void RedoParams();

protected:
	UPROPERTY()
	TObjectPtr<ABeamCharacter> Character;

	UPROPERTY()
	TObjectPtr<UBeamCharacterStateMachine> StateMachine;

};
