// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/BeamCharacterStateWalk.h"

#include "Characters/BeamCharacter.h"
#include "Characters/BeamCharacterStateMachine.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Characters/BeamCharacterSettings.h"

EBeamCharacterStateID UBeamCharacterStateWalk::GetStateID()
{
	return EBeamCharacterStateID::Walk;
}

void UBeamCharacterStateWalk::StateEnter(EBeamCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.f,
	// 	FColor::Red,
	// 	FString::Printf(TEXT("Enter State %d"), GetStateID())
	// );

	Character->GetCharacterMovement()->MaxWalkSpeed = Character->GetCharacterSettings()->Walk_VelocityMax;
}

void UBeamCharacterStateWalk::StateExit(EBeamCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.f,
	// 	FColor::Red,
	// 	FString::Printf(TEXT("Exit State %d"), GetStateID())
	// );
}

void UBeamCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	0.1f,
	// 	FColor::Red,
	// 	FString::Printf(TEXT("Tick State %d"), GetStateID())
	// );

	/*if (IsKeyWasPressed(EKeys::O)) {
		Character->SetStunTime(5.f);
		StateMachine->ChangeState(EBeamCharacterStateID::Stun);
	}*/

	if (Character->GetInputPush() && Character->CanPush() && !Character->isShooting()) {
		StateMachine->ChangeState(EBeamCharacterStateID::Push);
	}

	if (Character->GetInputJump() && !Character->isShooting()) {
		StateMachine->ChangeState(EBeamCharacterStateID::Jump);
		return;
	}

	if (!Character->GetMovementComponent()->IsMovingOnGround() && !Character->isShooting()) {
		StateMachine->ChangeState(EBeamCharacterStateID::Fall);
	}

	if (Character->GetInputMove().X == 0)
	{
		StateMachine->ChangeState(EBeamCharacterStateID::Idle);
	}
	else {
		/*if ((IsKeyDown(EKeys::Q) || Character->GetInputMove().X < 0) && Character->GetOrientX() == 1)
		{
			Character->SetOrientX(-1);
		}
		else if ((IsKeyDown(EKeys::D) || Character->GetInputMove().X > 0) && Character->GetOrientX() == -1)
		{
			Character->SetOrientX(1);
		}*/

		if (Character->isShooting()) {
			Character->GetCharacterMovement()->MaxWalkSpeed = Character->GetCharacterSettings()->Walk_VelocityMax / 6;
		}
		else {
			Character->GetCharacterMovement()->MaxWalkSpeed = Character->GetCharacterSettings()->Walk_VelocityMax;
		}
		
		float appliedForce = .0f;
		
		if (Character->GetInputMove().X != 0)
			appliedForce = Character->GetInputMove().X;
		else
			appliedForce = Character->GetOrientX();
		
		Character->AddMovementInput(FVector::ForwardVector, appliedForce);
	}


}

