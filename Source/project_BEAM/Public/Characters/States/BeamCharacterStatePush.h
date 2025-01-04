// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BeamCharacterState.h"
#include "BeamCharacterStatePush.generated.h"

class UAkAudioEvent;

UCLASS(ClassGroup = (BeamCharacterState), meta = (BlueprintSpawnableComponent))
class PROJECT_BEAM_API UBeamCharacterStatePush : public UBeamCharacterState
{
	GENERATED_BODY()
	
public:
	virtual EBeamCharacterStateID GetStateID() override;

	virtual void StateEnter(EBeamCharacterStateID PreviousStateID) override;

	virtual void StateExit(EBeamCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAkAudioEvent> GotPushedSound;
	
private:
	UPROPERTY() float timerPush = 0.0f;
	UPROPERTY(EditAnywhere) float timeToWait = 0.2f;


};
