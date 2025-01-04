// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "ArenaCamera.generated.h"

UCLASS()
class PROJECT_BEAM_API AArenaCamera : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArenaCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	TArray<AActor*> players;
};
