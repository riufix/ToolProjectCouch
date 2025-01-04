// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimeDilationManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BEAM_API ATimeDilationManager : public AActor
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void GoToDilation(float dilationToGo, float speed);

	UFUNCTION(BlueprintCallable)
	void GoToDilationWithWaitTime(float dilationToGo, float speed, float waitTime);

	UFUNCTION(BlueprintCallable)
	void StopDilation();

	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
	float Timer = .0f;

	UPROPERTY()
	float TimeToWait;

	UPROPERTY()
	bool isTimer = false;

	UPROPERTY()
	bool reversing = false;

	UPROPERTY()
	bool isWorking = false;

	UPROPERTY()
	bool achieved = false;

	UPROPERTY()
	float speed = 0;

	UPROPERTY()
	float dilationTarget;

};
