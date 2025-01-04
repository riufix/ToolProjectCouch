// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/BeamCharacterStateJump.h"

#include "Characters/BeamCharacterStateMachine.h"

#include "Characters/BeamCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Characters/BeamCharacterSettings.h"

#include "AkGameplayStatics.h"
#include "AkGameplayTypes.h"


EBeamCharacterStateID UBeamCharacterStateJump::GetStateID()
{
	return EBeamCharacterStateID::Jump;
}

void UBeamCharacterStateJump::StateEnter(EBeamCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->OnJump();

	Character->GetCharacterMovement()->JumpZVelocity = Character->GetCharacterSettings()->Jump_Force;
	Character->GetCharacterMovement()->MaxWalkSpeed = Character->GetCharacterSettings()->Jump_VelocityMax;

	if (PreviousStateID == EBeamCharacterStateID::Fall) {
		Character->LaunchCharacter(FVector(0,0,600), false, false);
	}
	else {
		Character->Jump();
	}

	FirstFrame = true;

	const FOnAkPostEventCallback nullCallback;
	UAkGameplayStatics::PostEvent(
			JumpingSound,
			Character,
			0,
			nullCallback,
			false
	);
	
	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.f,
	// 	FColor::Blue,
	// 	FString::Printf(TEXT("Enter State %d"), GetStateID())
	// );
}

void UBeamCharacterStateJump::StateExit(EBeamCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.f,
	// 	FColor::Blue,
	// 	FString::Printf(TEXT("Exit State %d"), GetStateID())
	// );
}

void UBeamCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	 // GEngine->AddOnScreenDebugMessage(
	 // 	-1,
	 // 	0.1f,
	 // 	FColor::Blue,
	 // 	FString::Printf(TEXT("STATE TICK JUMP"))
	 // );
	
	if (Character->GetInputMove().X != 0) 
	{

		float appliedForce = .0f;
		if (Character->GetInputMove().X != 0)
		{
			appliedForce = Character->GetInputMove().X;
		}
		else
		{
			appliedForce = Character->GetOrientX();
		}

		Character->AddMovementInput(FVector::ForwardVector, appliedForce);

	}



	if (Character->GetMovementComponent()->Velocity.Z < 0 && FirstFrame == false) {
		StateMachine->ChangeState(EBeamCharacterStateID::Fall);
		return;
	}

	if (Character->GetMovementComponent()->IsMovingOnGround() && FirstFrame == false) {
		StateMachine->ChangeState(EBeamCharacterStateID::Fall);
		return;
	}

	if (Character->GetInputPush() && Character->CanPush()) {
		StateMachine->ChangeState(EBeamCharacterStateID::Push);
		return;
	}

	if (FirstFrame) {
		FirstFrame = false;
	}
}
