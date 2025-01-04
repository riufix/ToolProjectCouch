// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena/ArenaCamera.h"
#include "Camera/CameraComponent.h"
#include "Characters/BeamCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AArenaCamera::AArenaCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	RootComponent = CameraComp;
}

// Called when the game starts or when spawned
void AArenaCamera::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABeamCharacter::StaticClass(), players);
}

// Called every frame
void AArenaCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

