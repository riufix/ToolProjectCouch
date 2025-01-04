// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BeamCharacterStateMachine.generated.h"

enum class EBeamCharacterStateID : uint8;
class UBeamCharacterState;
class ABeamCharacter;

UCLASS()
class PROJECT_BEAM_API UBeamCharacterStateMachine : public UObject
{
	GENERATED_BODY()
	
public:
	void Init(ABeamCharacter* Character);
	void Tick(float DeltaTime);

	UFUNCTION()
	ABeamCharacter* GetCharacter() const;

	UFUNCTION(BlueprintCallable)
	void ChangeState(EBeamCharacterStateID NewStateID);
	UFUNCTION(BlueprintCallable)
	UBeamCharacterState* GetState(EBeamCharacterStateID StateID);
	UFUNCTION()
	EBeamCharacterStateID const GetCurrentStateID() const;

	UFUNCTION()
	void RedoParams();

	UFUNCTION(BlueprintCallable)
	void SetCanChangeState(bool canChange);

	UFUNCTION(BlueprintCallable)
	bool GetCanChangeState() const;

protected: 
	UPROPERTY()
	TObjectPtr<ABeamCharacter> Character;
	
	UPROPERTY()
	TArray<UBeamCharacterState*> AllStates;
	UPROPERTY(BlueprintReadOnly)
	EBeamCharacterStateID CurrentStateID;
	UPROPERTY()
	TObjectPtr<UBeamCharacterState> CurrentState;

	UFUNCTION()
	void FindStates();
	UFUNCTION()
	void InitStates();

	UPROPERTY()
	bool canChangeState = true;
};
