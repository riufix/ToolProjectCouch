// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BeamCharacterInputData.generated.h"

class UInputAction;

UCLASS()
class PROJECT_BEAM_API UBeamCharacterInputData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionMoveVector2D;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionJump;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionDash;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionCharge;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionAimVector2D;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionShoot;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionPunch;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionFly;
};
