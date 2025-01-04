// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectileInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleWall.generated.h"

UCLASS()
class PROJECT_BEAM_API ADestructibleWall : public AActor, public IProjectileInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADestructibleWall();

	virtual EProjectileType ProjectileGetType() override;
	virtual AProjectile* GetProjectile() override;
	virtual bool ProjectileContext(int power, FVector position) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int resistance;
	
	UFUNCTION()
	void GetDestroyed();
	
};
