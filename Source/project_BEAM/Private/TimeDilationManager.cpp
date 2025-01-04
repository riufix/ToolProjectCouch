// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeDilationManager.h"
#include <Kismet/GameplayStatics.h>

void ATimeDilationManager::GoToDilation(float dilationToGo, float speedIn)
{
	isWorking = true;
	dilationTarget = dilationToGo;
	speed = speedIn;
	isTimer = false;
	achieved = false;
	reversing = false;
}

void ATimeDilationManager::GoToDilationWithWaitTime(float dilationToGo, float speedIn, float waitTime)
{
	isWorking = true;
	dilationTarget = dilationToGo;
	speed = speedIn;
	TimeToWait = waitTime;
	isTimer = true;
	achieved = false;
	reversing = false;
	Timer = 0;

}

void ATimeDilationManager::StopDilation()
{
	isWorking = false;

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
}

void ATimeDilationManager::Tick(float DeltaTime)
{

	if (!isWorking) return;

	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, TEXT("Working Time Dilation"));

	float timeDilationValue = 1.f;

	switch (isTimer) {


	// TIMER
	case true:

		if (achieved) {
			Timer += DeltaTime;
			GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, TEXT("Timer Time Dilation"));
			timeDilationValue = FMath::FInterpTo(UGameplayStatics::GetGlobalTimeDilation(GetWorld()), dilationTarget, DeltaTime, speed);
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), timeDilationValue);

			if (Timer >= TimeToWait) {
				Timer = 0;
				achieved = false;
				reversing = true;
			}
		}

		if (achieved) return;

		if (reversing) {
			timeDilationValue = FMath::FInterpTo(UGameplayStatics::GetGlobalTimeDilation(GetWorld()), 1.f, DeltaTime, speed);
		}
		else {
			timeDilationValue = FMath::FInterpTo(UGameplayStatics::GetGlobalTimeDilation(GetWorld()), dilationTarget, DeltaTime, speed);
			if (timeDilationValue <= dilationTarget) achieved = true;


		}

		if (!achieved) UGameplayStatics::SetGlobalTimeDilation(GetWorld(), timeDilationValue);

		if (timeDilationValue >= 1.f && reversing) isWorking = false;
		break;
		

	// NO TIMER
	case false:
		if (reversing) {
			timeDilationValue = FMath::FInterpTo(UGameplayStatics::GetGlobalTimeDilation(GetWorld()), 1.f, DeltaTime, speed);
			
		}
		else {
			timeDilationValue = FMath::FInterpTo(UGameplayStatics::GetGlobalTimeDilation(GetWorld()), dilationTarget, DeltaTime, speed);
			if (timeDilationValue <= dilationTarget) reversing = true;
		}
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), timeDilationValue);


		if (timeDilationValue >= 1.f && reversing) isWorking = false;
		break;


	}

	if (timeDilationValue >= 1.f) isWorking = false;

	
}

