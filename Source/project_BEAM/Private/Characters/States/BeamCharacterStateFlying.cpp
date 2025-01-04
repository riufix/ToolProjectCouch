// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/BeamCharacterStateFlying.h"

#include "Characters/BeamCharacter.h"	
#include "Characters/BeamCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/BeamCharacterSettings.h"

#include "AkGameplayStatics.h"
#include "AkGameplayTypes.h"


EBeamCharacterStateID UBeamCharacterStateFlying::GetStateID()
{
	return EBeamCharacterStateID::Fly;
}

void UBeamCharacterStateFlying::StateEnter(EBeamCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	firstFrame = true;

	Character->GetCharacterMovement()->BrakingFrictionFactor = Character->GetCharacterSettings()->Fly_BrakingFrictionFactor;

	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.f,
	// 	FColor::Red,
	// 	FString::Printf(TEXT("Enter State %d"), GetStateID())
	// );

	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void UBeamCharacterStateFlying::StateExit(EBeamCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	Character->GetCharacterMovement()->BrakingFrictionFactor = Character->GetCharacterSettings()->BrakingFrictionFactor;

	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.f,
	// 	FColor::Red,
	// 	FString::Printf(TEXT("Exit State %d"), GetStateID())
	// );

	firstFrame = true;
}

void UBeamCharacterStateFlying::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	if (!Character->IsPhaseTwo()) {
		StateMachine->ChangeState(EBeamCharacterStateID::Idle);
		return;
	}

	if (Character->GetInputPush() && Character->CanPush()) {
		StateMachine->ChangeState(EBeamCharacterStateID::Push);
	}

	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	0.1f,
	// 	FColor::Blue,
	// 	FString::Printf(TEXT("STATE TICK FLY"))
	// );

	if (!canDash) {
		timerDash += DeltaTime;
		if (timerDash >= Character->GetCharacterSettings()->Fly_DashTimer) {
			canDash = true;
			timerDash = 0;
		}
	}

	if (!canMove) {
		timerInputs += DeltaTime;
		if (timerInputs >= Character->GetCharacterSettings()->Fly_InputsTimer) {
			canMove = true;
			Character->SetIsDashing(false);
			timerInputs = 0;
		}
	}

	
	// Dash
	if (Character->GetInputDash() && !Character->GetIsDashing() && canDash) {

		Character->SetIsDashing(true);
		canDash = false;
		canMove = false;
		
		// GEngine->AddOnScreenDebugMessage(
		// 	-1,
		// 	2.0f,
		// 	FColor::Blue,
		// 	FString::Printf(TEXT("Dash %d"), GetStateID())
		// );
		
		FVector dashVector = FVector::ZeroVector;
		if (Character->GetInputMove() != FVector2D::ZeroVector)
		{
			dashVector = FVector(Character->GetInputMove().X,0,Character->GetInputMove().Y);
		}

		if (dashVector == FVector::ZeroVector)
		{
			Character->SetIsDashing(false);
			canDash = true;
			canMove = true;
			return;
		}
		else
		{
			const FOnAkPostEventCallback nullCallback;
			UAkGameplayStatics::PostEvent(
					DashSound,
					Character,
					0,
					nullCallback,
					false
			);
		}
		
		Character->GetCharacterMovement()->AddImpulse(dashVector * Character->GetCharacterSettings()->Fly_DashForce );
	}

	if (canMove)
	{
		//Character->SetOrientX(Character->GetInputMove().X);
	
		if (Character->GetInputMove() != FVector2D::ZeroVector)
		{
			FVector moveVector = FVector(Character->GetInputMove().X,0,Character->GetInputMove().Y);
			moveVector.Normalize();
			Character->AddMovementInput(moveVector, Character->GetInputMove().Length());
		}
	}
}

void UBeamCharacterStateFlying::RedoParams()
{
	Character->GetCharacterMovement()->BrakingFrictionFactor = Character->GetCharacterSettings()->Fly_BrakingFrictionFactor;
}

//void UBeamCharacterStateFlying::AfterDash()
//{
//	canDash = true;
//	canMove = true;
//}
