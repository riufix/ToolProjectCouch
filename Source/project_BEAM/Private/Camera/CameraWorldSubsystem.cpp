// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"


#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include <Kismet/GameplayStatics.h>
#include <Camera/CameraFollowTarget.h>
#include "Components/BoxComponent.h"
#include <Camera/BeamCameraSettings.h>

void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	CameraZoomDistanceBetweenTargetsMin = GetDefault<UBeamCameraSettings>()->CameraZoomDistanceBetweenTargetsMin;
	CameraZoomDistanceBetweenTargetsMax = GetDefault<UBeamCameraSettings>()->CameraZoomDistanceBetweenTargetsMax;

	ArenaCamera = Cast<AArenaCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), AArenaCamera::StaticClass()));

	CameraMain = FindCameraByTag(TEXT("CameraMain"));

	if (ArenaCamera == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("ArenaCamera not found"));
		return;
	}

	if (CameraMain == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("CameraMain not found"));
		return;

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Camera FOUND !"));
	}

	rotCameraStart = ArenaCamera->GetActorRotation();

	USceneComponent* ParentComponent = CameraMain->GetAttachParent();

	if (ParentComponent)
	{
		// Affiche le nom du parent dans les logs
		UE_LOG(LogTemp, Warning, TEXT("Parent of CameraMain: %s"), *ParentComponent->GetName());
	}
	else
	{
		// Si aucun parent n'est trouvé
		UE_LOG(LogTemp, Warning, TEXT("CameraMain has no parent."));
	}

	AActor* CameraBoundsActor = FindCameraBoundsActor();

	if (CameraBoundsActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Camerabounds true"));
		InitCameraBounds(CameraBoundsActor);
		FVector posCamera = ArenaCamera->GetActorLocation();
		ArenaCamera->SetActorLocation(FVector(CameraMain->Bounds.Origin.X, posCamera.Y, CameraMain->Bounds.Origin.Z));
	}

	InitCameraZoomParameters();

}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (CameraMain != nullptr)
	{
		switch (cameraMode)
		{
		case ECameraMode::None:
			break;
		case ECameraMode::Follow:
			CameraFollowMode(DeltaTime);
			TickUpdateCameraZoom(DeltaTime);
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, TEXT("TICK FOLLOW"));
			break;

		case ECameraMode::Cinematic:
			CameraCinematicMode(DeltaTime);
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, TEXT("TICK CINEMATIC"));
			break;
		case ECameraMode::Free:
			CameraFreeMode(DeltaTime);
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, TEXT("TICK FREE"));
			break;

		default:
			break;
		}
	}
	//TickUpdateCameraPosition(DeltaTime);
}

void UCameraWorldSubsystem::AddFollowTarget(UObject* FollowTarget)
{
	if (FollowTarget)
	{
		FollowTargets.Add(FollowTarget);
	}
}

void UCameraWorldSubsystem::RemoveFollowTarget(UObject* FollowTarget)
{
	if (FollowTarget)
	{
		FollowTargets.Remove(FollowTarget);
	}
}

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
}

void UCameraWorldSubsystem::TickUpdateCameraZoom(float DeltaTime)
{
	if (CameraMain == nullptr) return;

	if (ArenaCamera == nullptr) return;

	float GreatestDistanceBetweenTargets = CalculateGreatestDistanceBetweenTargets();
	//UE_LOG(LogTemp, Warning, TEXT("Greatest Distance : %f"), GreatestDistanceBetweenTargets);

	if (GreatestDistanceBetweenTargets < CameraZoomDistanceBetweenTargetsMin) {
		GreatestDistanceBetweenTargets = CameraZoomDistanceBetweenTargetsMin;
	}

	float PercentDistance = 1 - CameraZoomDistanceBetweenTargetsMax / GreatestDistanceBetweenTargets;

	PercentDistance = FMath::Clamp(PercentDistance, 0, 1);

	//float PercentDistance = FMath::Lerp(0.0f, 1.0f, FMath::Clamp(GreatestDistanceBetweenTargets, CameraZoomDistanceBetweenTargetsMin, CameraZoomDistanceBetweenTargetsMax));

	FVector posCamera = ArenaCamera->GetActorLocation();

	//UE_LOG(LogTemp, Warning, TEXT("PERCENTAGE : %f"), PercentDistance);

	float value = CameraZoomYMax * PercentDistance;

	//UE_LOG(LogTemp, Warning, TEXT("VALUE : %f"), value);

	float zoomValue = FMath::Clamp(value, CameraZoomYMin, CameraZoomYMax);

	//UE_LOG(LogTemp, Warning, TEXT("ZOOM VALUE : %f"), zoomValue);


	FVector newPosition = FVector(posCamera.X, zoomValue, posCamera.Z);


	//UE_LOG(LogTemp, Warning, TEXT("POSITION NEW ZOOM : %s"), *newPosition.ToString());

	ArenaCamera->SetActorLocation(newPosition);

	CameraMain->SetWorldLocation(FVector(0.0f, FMath::Lerp(CameraZoomYMin, CameraZoomYMax, PercentDistance), 0.0f));

}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{

	FVector averagePosition = FVector::ZeroVector;

	int NumFollowTargets = 0;

	for (UObject* FollowTarget : FollowTargets)
	{
		if (FollowTarget == nullptr) continue;

		TScriptInterface<ICameraFollowTarget> FollowTargetInterface = FollowTarget;

		if (FollowTargetInterface == nullptr) continue;

		if (!FollowTargetInterface->IsFollowable()) continue;

		averagePosition += FollowTargetInterface->GetFollowPosition();

		NumFollowTargets++;

	}

	if (NumFollowTargets > 0)
	{
		averagePosition /= NumFollowTargets;
		//UE_LOG(LogTemp, Warning, TEXT("averagePosition: X=%f, Y=%f, Z=%f"), averagePosition.X, averagePosition.Y, averagePosition.Z);

		return averagePosition;
	}


	return FVector::ZeroVector;
}

float UCameraWorldSubsystem::CalculateGreatestDistanceBetweenTargets()
{

	float GreatestDistance = 0.0f;

	for (int i = 0; i < FollowTargets.Num(); i++)
	{
		if (FollowTargets[i] == nullptr) continue;


		for (int j = 0; j < FollowTargets.Num(); j++) {

			if (FollowTargets[j] == nullptr) continue;
			if (i == j) continue;

			TScriptInterface<ICameraFollowTarget> FollowTargetInterface = FollowTargets[i];

			if (FollowTargetInterface == nullptr) continue;
			if (!FollowTargetInterface->IsFollowable()) continue;

			TScriptInterface<ICameraFollowTarget> FollowTargetInterface2 = FollowTargets[j];

			if (FollowTargetInterface2 == nullptr) continue;
			if (!FollowTargetInterface2->IsFollowable()) continue;

			//UE_LOG(LogTemp, Error, TEXT("FollowTargetInterface: %s"), *FollowTargetInterface->GetFollowPosition().ToString());
			//UE_LOG(LogTemp, Error, TEXT("FollowTargetInterface2: %s"), *FollowTargetInterface2->GetFollowPosition().ToString());

			float distance = FVector::Distance(FollowTargetInterface->GetFollowPosition(), FollowTargetInterface2->GetFollowPosition());

			//UE_LOG(LogTemp, Error, TEXT("Distance: %f"), distance);

			if (distance > GreatestDistance)
			{
				GreatestDistance = distance;
			}

		}

	}


	return GreatestDistance;
}



UCameraComponent* UCameraWorldSubsystem::FindCameraByTag(const FName& Tag) const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundActors);

	if (FoundActors.Num() > 0)
	{
		return FoundActors[0]->FindComponentByClass<UCameraComponent>();
	}

	return nullptr;
}

AActor* UCameraWorldSubsystem::FindCameraBoundsActor()
{

	TArray<AActor*> CameraBoundsActors;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "CameraBounds", CameraBoundsActors);

	if (CameraBoundsActors[0] == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Bounds not found"));

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Bounds FOUND !"));
	}

	return CameraBoundsActors[0];
}

void UCameraWorldSubsystem::InitCameraBounds(AActor* CameraBoundsActor)
{
	FVector BoundsCenter;
	FVector BoundsExtent;

	CameraBoundsActor->GetActorBounds(false, BoundsCenter, BoundsExtent);

	//TArray<UBoxComponent*> components;

	//CameraBoundsActor->GetComponents<UBoxComponent>(components);

	UE_LOG(LogTemp, Warning, TEXT("CameraOrigin: %s"), *CameraMain->Bounds.Origin.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Black, FString::Printf(TEXT("BoxOrigin: %s"), *CameraMain->Bounds.Origin.ToString()));
	CameraMain->Bounds.BoxExtent = BoundsExtent;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("BoxExtent: %s"), *CameraMain->Bounds.BoxExtent.ToString()));
	CameraMain->Bounds.Origin = BoundsCenter;

	Origin = BoundsCenter;

	CameraBoundsMin = FVector2D(BoundsCenter.X - BoundsExtent.X, BoundsCenter.Z - BoundsExtent.Z);
	CameraBoundsMax = FVector2D(BoundsCenter.X + BoundsExtent.X, BoundsCenter.Z + BoundsExtent.Z);

	//CameraMain->UpdateBounds();

}

void UCameraWorldSubsystem::ClampPositionIntoCameraBounds(FVector& Position)
{

	FVector2D ViewportBoundsMin, ViewportBoundsMax;

	GetViewportBounds(ViewportBoundsMin, ViewportBoundsMax);

	Position.X = FMath::Clamp(Position.X, CameraBoundsMin.X, CameraBoundsMax.X);
	Position.Z = FMath::Clamp(Position.Z, CameraBoundsMin.Y, CameraBoundsMax.Y);
}

void UCameraWorldSubsystem::GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax)
{
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient == nullptr) return;

	FViewport* Viewport = ViewportClient->Viewport;
	if (Viewport == nullptr) return;

	FIntRect ViewRect(
		Viewport->GetInitialPositionXY(),
		Viewport->GetInitialPositionXY() + Viewport->GetSizeXY()
	);

	FIntRect ViewportRect = Viewport->CalculateViewExtents(CameraMain->AspectRatio, ViewRect);


	OutViewportBoundsMin.X = ViewportRect.Min.X;
	OutViewportBoundsMin.Y = ViewportRect.Min.Y;

	OutViewportBoundsMax.X = ViewportRect.Max.X;
	OutViewportBoundsMax.Y = ViewportRect.Max.Y;

}

FVector UCameraWorldSubsystem::CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition)
{
	if (CameraMain == nullptr) return FVector::Zero();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController == nullptr) return FVector::Zero();

	float YDistanceToCenter = CameraMain->GetOwner()->GetActorLocation().Y - CameraBoundsYProjectionCenter;

	FVector CameraWorldProjectDir;
	FVector WorldPosition;

	UGameplayStatics::DeprojectScreenToWorld(
		PlayerController,
		ViewportPosition,
		WorldPosition,
		CameraWorldProjectDir

	);

	WorldPosition += CameraWorldProjectDir + YDistanceToCenter;

	return WorldPosition;
}

void UCameraWorldSubsystem::InitCameraZoomParameters()
{

	//TArray<AActor*> CameraDistancesMinActors;
	//TArray<AActor*> CameraDistancesMaxActors;


	//UGameplayStatics::GetAllActorsWithTag(GetWorld(), "CameraDistanceMin", CameraDistancesMinActors);
	//UGameplayStatics::GetAllActorsWithTag(GetWorld(), "CameraDistanceMax", CameraDistancesMaxActors);

	//if (CameraDistancesMinActors.Num() == 0 || CameraDistancesMaxActors.Num() == 0) return;

	//UE_LOG(LogTemp, Warning, TEXT("CameraDistancesMinActors: %d"), CameraDistancesMinActors[0]->GetActorLocation().Y);
	//UE_LOG(LogTemp, Warning, TEXT("CameraDistancesMaxActors: %d"), CameraDistancesMaxActors[0]->GetActorLocation().Y);

	CameraZoomYMin = GetDefault<UBeamCameraSettings>()->CameraZoomYMin;
	CameraZoomYMax = GetDefault<UBeamCameraSettings>()->CameraZoomYMax;

}

//void UCameraWorldSubsystem::SetShakeForce(float NewShakeForce)
//{
//	shakeForce = NewShakeForce;
//}

//void UCameraWorldSubsystem::SetCameraSpeed(float NewCameraSpeed)
//{
//	cameraSpeed = NewCameraSpeed;
//}

//void UCameraWorldSubsystem::ChangeCameraMode(ECameraMode NewCameraMode)
//{
//	cameraMode = NewCameraMode;
//}

//void UCameraWorldSubsystem::ChangeCameraFollowMode(ECameraFollowMode NewCameraFollowMode)
//{
//	isTimer = false;
//	timer = 0;
//
//	cameraFollowMode = NewCameraFollowMode;
//}

void UCameraWorldSubsystem::ShakeForSeconds(float Seconds, float ForceShake = 100)
{
	timerMax = Seconds;
	timer = 0;
	shakeForce = ForceShake;
	isTimer = true;
	cameraFollowMode = ECameraFollowMode::Shake;
}

void UCameraWorldSubsystem::ShakeCamera(float ForceShake, float speedCamera)
{
	if (cameraMode != ECameraMode::Follow) return;


	shakeForce = ForceShake;
	//cameraMode = ECameraMode::Follow;
	cameraFollowMode = ECameraFollowMode::Shake;
	isTimer = false;
	timer = 0;
	cameraSpeed = speedCamera;
}

void UCameraWorldSubsystem::UnShakeCamera()
{

	if (cameraFollowMode != ECameraFollowMode::Shake && cameraMode != ECameraMode::Follow) return;

	cameraFollowMode = ECameraFollowMode::Normal;
	isTimer = false;
	timer = 0;
	cameraSpeed = cameraBaseSpeed;
}

void UCameraWorldSubsystem::CinematicForSeconds(float Seconds, FVector PosToFollow, float CameraSpeed = 10)
{
	cameraSpeed = CameraSpeed;
	timer = 0;
	timerMax = Seconds;
	posToFollow = PosToFollow;
	//cameraMode = ECameraMode::Follow;
	cameraMode = ECameraMode::Cinematic;
	posToFollowStart = ArenaCamera->GetActorLocation();
	isReverse = false;
	isReversing = false;
	canReverse = true;
}



void UCameraWorldSubsystem::CameraCinematic(float CameraSpeed, FVector PosToFollow, AActor* InActorToFollow, bool canRotate, float yDistance)
{
	cameraSpeed = CameraSpeed;
	timer = 0;
	distanceY = yDistance;
	posToFollow = PosToFollow;
	actorToFollow = InActorToFollow;
	//cameraMode = ECameraMode::Follow;
	cameraMode = ECameraMode::Cinematic;
	changeRotation = canRotate;
	isReverse = false;
	isReversing = false;
	canReverse = false;
}

void UCameraWorldSubsystem::Cinematic(float CameraSpeed, FVector PosToFollow)
{
	cameraSpeed = CameraSpeed;
	timer = 0;
	posToFollow = PosToFollow;
	//cameraMode = ECameraMode::Follow;
	cameraMode = ECameraMode::Cinematic;
	isReverse = false;
	isReversing = false;
	canReverse = false;
}

void UCameraWorldSubsystem::CameraReverseCinematic(float CameraSpeed)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, TEXT("T"));
	cameraSpeed = CameraSpeed;
	timer = 0;
	cameraMode = ECameraMode::Cinematic;
	isReverse = true;
	isReversing = true;
	canReverse = false;
	rotToFollow = rotCameraStart;
}

void UCameraWorldSubsystem::ReverseCinematic(float CameraSpeed)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, TEXT("T"));
	cameraSpeed = CameraSpeed;
	timer = 0;
	cameraMode = ECameraMode::Cinematic;
	actorToFollow = nullptr;
	isReverse = true;
	isReversing = true;
	canReverse = false;
	rotToFollow = rotCameraStart;
}

void UCameraWorldSubsystem::CameraFollowMode(float DeltaTime)
{
	FVector NewCameraPosition = CalculateAveragePositionBetweenTargets();

	FVector posCamera = ArenaCamera->GetActorLocation();
	ClampPositionIntoCameraBounds(NewCameraPosition);

	posToFollow = NewCameraPosition;
	FRotator rotCamera = ArenaCamera->GetActorRotation();

	switch (cameraFollowMode)
	{
	case ECameraFollowMode::Normal:
		ArenaCamera->SetActorLocation(FMath::VInterpTo(posCamera, FVector(NewCameraPosition.X, posCamera.Y, NewCameraPosition.Z), DeltaTime, cameraSpeed));
		ArenaCamera->SetActorRotation(FMath::RInterpTo(ArenaCamera->GetActorRotation(), rotCameraStart, DeltaTime, cameraSpeed));

		break;
	case ECameraFollowMode::Shake:
		ArenaCamera->SetActorLocation(FMath::VInterpTo(posCamera, FVector(NewCameraPosition.X + FMath::RandRange(-shakeForce, shakeForce), posCamera.Y, NewCameraPosition.Z + FMath::RandRange(-shakeForce, shakeForce)), DeltaTime, cameraSpeed));
		break;
	default:
		break;
	}

	if (isTimer && cameraFollowMode == ECameraFollowMode::Shake) {
		timer += DeltaTime;
		if (timer >= timerMax) {
			isTimer = false;
			timer = 0;
			cameraSpeed = cameraBaseSpeed;
			cameraFollowMode = ECameraFollowMode::Normal;
		}
	}

	//ArenaCamera->SetActorRotation(rotToFollow);

	TickUpdateCameraZoom(DeltaTime);
}

void UCameraWorldSubsystem::CameraFreeMode(float DeltaTime)
{
}

void UCameraWorldSubsystem::CameraCinematicMode(float DeltaTime)
{
	FVector posCamera = ArenaCamera->GetActorLocation();
	FRotator lookAtRotation = FRotationMatrix::MakeFromX(posToFollow - posCamera).Rotator();

	if (isReverse && isReversing) {
		lookAtRotation = rotToFollow;
	}

	//UE_LOG(LogTemp, Warning, TEXT("TICK CAMERA MODE"));

	if (actorToFollow != nullptr) {
		posToFollow = actorToFollow->GetActorLocation();
		//UE_LOG(LogTemp, Warning, TEXT("POSITION ACTOR : %s"), *actorToFollow->GetActorLocation().ToString());
	}

	FVector posToGet = FVector(posToFollow.X, posToFollow.Y + distanceY, posToFollow.Z + 100);

	if (!changeRotation) {
		posToGet = FVector(posToFollow.X, posToFollow.Y + distanceY, posToFollow.Z);
	}

	if (isReverse && isReversing) {
		FVector NewCameraPosition = CalculateAveragePositionBetweenTargets();
		ClampPositionIntoCameraBounds(NewCameraPosition);
		
		float GreatestDistanceBetweenTargets = CalculateGreatestDistanceBetweenTargets();
		if (GreatestDistanceBetweenTargets < CameraZoomDistanceBetweenTargetsMin) {
			GreatestDistanceBetweenTargets = CameraZoomDistanceBetweenTargetsMin;
		}
		float PercentDistance = 1 - CameraZoomDistanceBetweenTargetsMax / GreatestDistanceBetweenTargets;
		PercentDistance = FMath::Clamp(PercentDistance, 0, 1);
		float value = CameraZoomYMax * PercentDistance;
		float zoomValue = FMath::Clamp(value, CameraZoomYMin, CameraZoomYMax);


		posToGet = FVector(NewCameraPosition.X, zoomValue, NewCameraPosition.Z);

		lookAtRotation = rotToFollow;
	}

	ArenaCamera->SetActorLocation(FMath::VInterpTo(posCamera, posToGet, DeltaTime, cameraSpeed));
	
	if (changeRotation) {
		ArenaCamera->SetActorRotation(FMath::RInterpTo(ArenaCamera->GetActorRotation(), lookAtRotation, DeltaTime, cameraSpeed));
	}

	if (
		posCamera.X > posToGet.X - 10 && posCamera.X < posToGet.X + 10 &&
		posCamera.Z > posToGet.Z - 10 && posCamera.Z < posToGet.Z + 10 &&
		isTimer == false && !isReverse
		) 
	{
		isTimer = true;
	}

	if (isReverse) {
		if (
			posCamera.X > posToGet.X - 1 && posCamera.X < posToGet.X + 1 &&
			posCamera.Z > posToGet.Z - 1 && posCamera.Z < posToGet.Z + 1 &&
			isTimer == false
			)
		{
			if (isReversing) {
				isTimer = false;
				timer = 0;
				cameraSpeed = cameraBaseSpeed;
				rotToFollow = rotCameraStart;
				cameraMode = ECameraMode::Follow;
				cameraFollowMode = ECameraFollowMode::Normal;
				isReverse = false;
				isReversing = false;
			}
			else {
				isTimer = true;
			}
		}

		if (canReverse) {
			if (isTimer) {
				timer += DeltaTime;
				if (timer >= timerMax) {
					isTimer = false;
					timer = 0;
					rotToFollow = rotCameraStart;
					posToFollow = posToFollowStart;
					isReversing = true;
				}
			}
		}
	}
	else {
		if (canReverse) {
			if (isTimer) {
				timer += DeltaTime;
				if (timer >= timerMax) {

					isTimer = false;
					timer = 0;
					cameraSpeed = cameraBaseSpeed;
					rotToFollow = rotCameraStart;
					cameraMode = ECameraMode::Follow;
					cameraFollowMode = ECameraFollowMode::Normal;
				
				}
			}
		}
	}
}



