// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/BeamCharacterStateProjection.h"
#include "Characters/BeamCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/BeamCharacterStateMachine.h"
#include "Characters/BeamCharacterSettings.h"

#include "AkGameplayStatics.h"
#include "AkGameplayTypes.h"

EBeamCharacterStateID UBeamCharacterStateProjection::GetStateID()
{
	return EBeamCharacterStateID::Projection;
}

void UBeamCharacterStateProjection::StateEnter(EBeamCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	
	Character->SetCanTakeDamage(false);
	Character->SetCanTakeKnockback(true);
	TimeToWait = Character->GetCharacterSettings()->TimeToWaitAfterProjection;
	Timer = 0.f;
	TimeLeft = 0.f;
	AfterProjection = false;

	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.f,
	// 	FColor::Blue,
	// 	FString::Printf(TEXT("Enter State %d"), GetStateID())
	// );

	const FOnAkPostEventCallback nullCallback;
	UAkGameplayStatics::PostEvent(
			ProjectionSound,
			Character,
			0,
			nullCallback,
			false
	);
	
	Character->ChangeColorToWhite();
}

void UBeamCharacterStateProjection::StateExit(EBeamCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	
	Character->SetCanTakeDamage(true);
	Character->SetCanTakeKnockback(true);

	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.f,
	// 	FColor::Blue,
	// 	FString::Printf(TEXT("Exit State %d"), GetStateID())
	// );

	Character->ChangeColorToNormal();
}

void UBeamCharacterStateProjection::StateTick(float DeltaTime)
{

	 // GEngine->AddOnScreenDebugMessage(
	 // 	-1,
	 // 	0.1f,
	 // 	FColor::Red,
	 // 	FString::Printf(TEXT("Tick State PROJECTION"))
	 // );

	TimeLeft += DeltaTime;
	if (TimeLeft >= TimeLeftMax && !AfterProjection) AfterProjection = true; 
	
	if (AfterProjection) {
		Timer += DeltaTime;
		if (Timer >= TimeToWait) {

			AfterProjection = false;
			Character->SetCanTakeDamage(true);
			Character->SetCanTakeKnockback(true);
			Timer = 0.f;
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("After Projection END"));
			StateMachine->ChangeState(EBeamCharacterStateID::Idle);
		}
	}

	if (Character->GetCharacterMovement()->IsMovingOnGround() && Character->GetCharacterMovement()->GetLastUpdateVelocity().Size() <= Character->GetMinSizeVelocity()) 
	{
		AfterProjection = true;
		Character->SetCanTakeKnockback(false);
	}

}
