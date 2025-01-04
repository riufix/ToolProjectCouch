// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/BeamCharacterStateStun.h"
#include "Characters/BeamCharacter.h"	
#include "Characters/BeamCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/BeamCharacterSettings.h"


EBeamCharacterStateID UBeamCharacterStateStun::GetStateID()
{
	return EBeamCharacterStateID::Stun;
}

void UBeamCharacterStateStun::StateEnter(EBeamCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	timeToStun = Character->GetStunTime();

	if (Character->IsPhaseTwo()) {
		Character->GetCharacterMovement()->MaxFlySpeed = Character->GetCharacterSettings()->Fly_MaxSpeed * static_cast<double>(Character->GetMultiplierStun());

		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	}
	else {

		Character->GetCharacterMovement()->MaxWalkSpeed = Character->GetCharacterSettings()->Walk_VelocityMax * static_cast<double>(Character->GetMultiplierStun());

		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.f,
	// 	FColor::Red,
	// 	FString::Printf(TEXT("Enter State %d"), GetStateID())
	// );
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		10.f,
		FColor::Blue,
		FString::Printf(TEXT("STUN TIME %f"), Character->GetStunTime())
	);
}

void UBeamCharacterStateStun::StateExit(EBeamCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	
	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.f,
	// 	FColor::Red,
	// 	FString::Printf(TEXT("Exit State %d"), GetStateID())
	// );

	if (Character->IsPhaseTwo()) {
		Character->GetCharacterMovement()->MaxFlySpeed = Character->GetCharacterSettings()->Fly_MaxSpeed;

		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	}
	else {
		Character->GetCharacterMovement()->MaxWalkSpeed = Character->GetCharacterSettings()->Walk_VelocityMax;
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

}

void UBeamCharacterStateStun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	stunTimer += DeltaTime;

	 // GEngine->AddOnScreenDebugMessage(
	 // 	-1,
	 // 	0.1f,
	 // 	FColor::Blue,
	 // 	FString::Printf(TEXT("STATE TICK STUN"))
	 // );

	if (stunTimer >= timeToStun) {
		stunTimer = 0.f;
		StateMachine->ChangeState(EBeamCharacterStateID::Idle);
	}

	if (Character->GetMultiplierStun() > 0.0f) {
		Character->SetOrientX(Character->GetInputMove().X);
	}

	if (Character->GetInputMove() != FVector2D::ZeroVector)
	{
		FVector moveVector = FVector(Character->GetInputMove().X, 0, Character->GetInputMove().Y);
		moveVector.Normalize();
		Character->AddMovementInput(moveVector, Character->GetInputMove().Length());
	}
	
}
