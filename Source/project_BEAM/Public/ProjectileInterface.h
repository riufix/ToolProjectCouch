// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectileInterface.generated.h"

class AProjectile;

UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	Player,
	Bullet,
	DestructWall
};

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class PROJECT_BEAM_API UProjectileInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT_BEAM_API IProjectileInterface
{
	GENERATED_BODY()

public:
	/** Return the type of the interactable Actor found */
	virtual EProjectileType ProjectileGetType() = 0;

	/** Contextual action depending on the Actor, returns if the Bullet must destroy itself */
	virtual bool ProjectileContext(int power, FVector position) = 0;
	
	/** If the actor found is a projectile, returns itself */
	virtual AProjectile* GetProjectile() = 0;
};