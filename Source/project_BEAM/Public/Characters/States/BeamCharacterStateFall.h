// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BeamCharacterState.h"
#include "BeamCharacterStateFall.generated.h"

class UAkAudioEvent;

UCLASS(ClassGroup = (BeamCharacterState), meta = (BlueprintSpawnableComponent))
class PROJECT_BEAM_API UBeamCharacterStateFall : public UBeamCharacterState
{
	GENERATED_BODY()

public:
	virtual EBeamCharacterStateID GetStateID() override;

	virtual void StateEnter(EBeamCharacterStateID PreviousStateID) override;

	virtual void StateExit(EBeamCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAkAudioEvent> LandingSound;
	
# pragma region Timer

private:
	UPROPERTY() float timerCoyote = 0.0f;
	UPROPERTY() float timerCoyoteMax = 0.2f;

	UPROPERTY() bool canCoyote = true;

	UFUNCTION() void OnCoyoteTimerEnd();
	UFUNCTION() void ResetCoyoteTimer();

	UPROPERTY() float ZVelocity = 0;
	
# pragma endregion

};
