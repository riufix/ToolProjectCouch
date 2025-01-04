// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BeamCharacter.h"

#include "Kismet/KismetSystemLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Characters/BeamCharacterStateMachine.h"
#include "Characters/BeamCharacterSettings.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/PlayerAim.h"
#include "Characters/BeamCharacterStateID.h"
#include "Components/BoxComponent.h"
#include "ProjectileInterface.h"
#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "WeaponCharge.h"

#include <Camera/CameraWorldSubsystem.h>
#include "ProjectileSettings.h"

#include "AkGameplayStatics.h"
#include "AkGameplayTypes.h"


// Sets default values
ABeamCharacter::ABeamCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

#pragma region Projectile Interface

EProjectileType ABeamCharacter::ProjectileGetType()
{
	return EProjectileType::Player;
}

bool ABeamCharacter::ProjectileContext(int power, FVector position)
{
	if (weaponComp->GetIsQteActive()) weaponComp->CancelWeaponCharge(true);
	PlayerTakeDamage(power + 1);

	FVector direction = GetActorLocation() - position;
	direction.Normalize();

	KnockBack(direction, GetCharacterSettings()->DamageKnockbacks[power], true);

	return true;
}

AProjectile* ABeamCharacter::GetProjectile()
{
	return nullptr;
}
#pragma endregion

// Called when the game starts or when spawned
void ABeamCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetSubsystem<UCameraWorldSubsystem>()->AddFollowTarget(this);
	InitCharacterSettings();
	SetupCollision();
	CreateStateMachine();
	InitStateMachine();

	InitWeaponAndAim();

	StartLocation = this->GetActorLocation();
	SetOrientX(1.f);
	RotateMeshUsingOrientX();

	SetLife(MaxLife);
}

// Called every frame
void ABeamCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

	if (StateMachine->GetCurrentStateID() == EBeamCharacterStateID::Dead) { 
		
		return; 
	}

	if (!isFreeze)
	{
		if (!IsPhaseTwo())
		{
			if (InputMove.X > .0f && GetOrientX() < .0f)
			{
				SetOrientX(1.f);
				RotateMeshUsingOrientX();
			}
			else if (InputMove.X < .0f && GetOrientX() > .0f)
			{
				SetOrientX(-1.f);
				RotateMeshUsingOrientX();
			}
		}
		else
		{
			if ((GetPlayerAimComp()->GetAimPos() - GetActorLocation()).X > .0f  && GetOrientX() < .0f)
			{
				SetOrientX(1.f);
				RotateMeshUsingOrientX();
			}
			else if ((GetPlayerAimComp()->GetAimPos() - GetActorLocation()).X < .0f  && GetOrientX() > .0f)
			{
				SetOrientX(-1.f);
				RotateMeshUsingOrientX();
			}
		}
	}
	
	if (StateMachine != nullptr) {
		if (StateMachine->GetCurrentStateID() != EBeamCharacterStateID::Projection) {
			SetCanTakeKnockback(true);
		}
	}

	// Check if he is shooting -> can't move if he is charging in phase 1
	//if (GetComponentByClass<UplayerAimComp>() != nullptr) {

	TickStateMachine(DeltaTime);

	TickPush(DeltaTime);

	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, GetName() + FString::Printf(TEXT(" current life : %d"), Life));

	if (GetActorLocation().Y != StartLocation.Y)
	{
		SetActorLocation(FVector(GetActorLocation().X, StartLocation.Y, GetActorLocation().Z));
	}

	if (InputJumpJoystick && (StateMachine->GetCurrentStateID() == EBeamCharacterStateID::Jump || StateMachine->GetCurrentStateID() == EBeamCharacterStateID::Fall))
	{
		InputJumpJoystick = false;
	}
}

/**---State Machine---*/
void ABeamCharacter::TickStateMachine(float DeltaTime) const
{
	if (StateMachine == nullptr) return;
	StateMachine->Tick(DeltaTime);
}

/**---Push---*/
void ABeamCharacter::TickPush(float DeltaTime)
{
	if (!canPush) {
		timerPush += DeltaTime;
		if (timerPush >= timeToWaitPush) {
			canPush = true;
			timerPush = 0;
		}
	}
}

// Called to bind functionality to input
void ABeamCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupMappingContextIntoController();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(EnhancedInputComponent == nullptr) return;

	BindInputActions(EnhancedInputComponent);
}

#pragma region Orient

void ABeamCharacter::RotateMeshUsingOrientX() const
{
	FRotator Rotation = GetMesh()->GetRelativeRotation();

	Rotation.Yaw = -90.f * OrientX;
	GetMesh()->SetRelativeRotation(Rotation);
}



#pragma endregion

#pragma region State Machine

void ABeamCharacter::CreateStateMachine()
{
	StateMachine = NewObject<UBeamCharacterStateMachine>(this);
}

void ABeamCharacter::InitStateMachine()
{
	if (StateMachine == nullptr) return;
	StateMachine->Init(this);
}
#pragma endregion

#pragma region Character Settings

void ABeamCharacter::InitCharacterSettings()
{
	
	CharacterSettings = GetDefault<UBeamCharacterSettings>();
	ProjectileSettings = GetDefault<UProjectileSettings>();
	
	if (ProjectileSettings != nullptr)
	{
		shootRadius.Empty();
		shootRadius.Add(ProjectileSettings->width_0 * 50.f);
		shootRadius.Add(ProjectileSettings->width_1 * 50.f);
		shootRadius.Add(ProjectileSettings->width_2 * 50.f);
		shootRadius.Add(ProjectileSettings->width_3 * 50.f);

		shootHalfHeight.Empty();
		shootHalfHeight.Add(ProjectileSettings->height_0 * 50.f);
		shootHalfHeight.Add(ProjectileSettings->height_1 * 50.f);
		shootHalfHeight.Add(ProjectileSettings->height_2 * 50.f);
		shootHalfHeight.Add(ProjectileSettings->height_3 * 50.f);
	}

	if (CharacterSettings == nullptr) return;

	// CHARACTER COMPONENT SETTINGS
	GetCharacterMovement()->MaxAcceleration = CharacterSettings->MaxAcceleration;
	GetCharacterMovement()->GroundFriction = CharacterSettings->GroundFriction;
	GetCharacterMovement()->GravityScale = CharacterSettings->GravityScale;
	GetCharacterMovement()->Mass = CharacterSettings->Mass;
	GetCharacterMovement()->BrakingDecelerationWalking = CharacterSettings->BreakingDecelerationWalking;
	GetCharacterMovement()->JumpZVelocity = CharacterSettings->Jump_Force;
	GetCharacterMovement()->AirControl = CharacterSettings->AirControl;
	GetCharacterMovement()->FallingLateralFriction = CharacterSettings->FallingLateralFriction;
	GetCharacterMovement()->MaxFlySpeed = CharacterSettings->Fly_MaxSpeed;
	
	// CHARACTER STATS SETTINGS
	MaxLife = CharacterSettings->MaxLife;
	Life = MaxLife;
	LifeToFly = CharacterSettings->LifeToFly;
	timeToWaitPush = CharacterSettings->Push_Cooldown;
	
}

void ABeamCharacter::ReattributeCharacterSettings()
{
	if (CharacterSettings == nullptr) return;

	GetCharacterMovement()->MaxAcceleration = CharacterSettings->MaxAcceleration;
	GetCharacterMovement()->GroundFriction = CharacterSettings->GroundFriction;
	GetCharacterMovement()->GravityScale = CharacterSettings->GravityScale;
	GetCharacterMovement()->Mass = CharacterSettings->Mass;
	GetCharacterMovement()->BrakingDecelerationWalking = CharacterSettings->BreakingDecelerationWalking;
	GetCharacterMovement()->JumpZVelocity = CharacterSettings->Jump_Force;
	GetCharacterMovement()->AirControl = CharacterSettings->AirControl;
	GetCharacterMovement()->FallingLateralFriction = CharacterSettings->FallingLateralFriction;
	GetCharacterMovement()->MaxFlySpeed = CharacterSettings->Fly_MaxSpeed;

	if (StateMachine != nullptr) {
		StateMachine->RedoParams();
	}
}
#pragma endregion

#pragma region Fight

void ABeamCharacter::KnockBack(FVector Direction, float Force, bool projection)
{

	if (!GetCanTakeKnockback()) {
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Emerald, FString::Printf(TEXT("KNOCKBACK REFUSED")));
	}

	if (!GetCanTakeKnockback()) return;

	GetCharacterMovement()->Launch(Direction * Force);
	if (projection) StateMachine->ChangeState(EBeamCharacterStateID::Projection);
}

void ABeamCharacter::Bounce(FVector Normal)
{

	FVector velocity = GetCharacterMovement()->GetLastUpdateVelocity();

	FVector velocityDir = velocity.GetSafeNormal();

	FVector newVector = velocityDir - 2 * FVector::DotProduct(velocityDir, Normal) * Normal;

	float Force = velocity.Size() * Bounciness;

	KnockBack(newVector, Force);

	const FOnAkPostEventCallback nullCallback;
	UAkGameplayStatics::PostEvent(
			BouncingSound,
			this,
			0,
			nullCallback,
			false
	);
}

void ABeamCharacter::OnHit(
	UPrimitiveComponent* HitComponent,  // The component that was hit
	AActor* OtherActor,                // The other actor involved in the hit
	UPrimitiveComponent* OtherComp,    // The other actor's component that was hit
	FVector NormalImpulse,             // The force applied to resolve the collision
	const FHitResult& Hit              // Detailed information about the hit
)
{
	if (OtherActor == nullptr) return;

	if (StateMachine->GetCurrentStateID() != EBeamCharacterStateID::Projection && StateMachine->GetCurrentStateID() != EBeamCharacterStateID::Fly) return;

	if (StateMachine->GetCurrentStateID() == EBeamCharacterStateID::Fly && !GetIsDashing()) return;

	FVector velocity = GetCharacterMovement()->GetLastUpdateVelocity();

	if (velocity.Size() < MinSizeVelocity) return;

	Bounce(Hit.Normal);
	
	OnHitWallProjection(Hit.Location);
	// EFFECT 

}
#pragma endregion

#pragma region Life
void ABeamCharacter::PlayerTakeDamage(const int Damage)
{

	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	5.f,
	// 	FColor::Purple,
	// 	FString::Printf(TEXT("TAKE DAMAGE"))
	// );
	
	const FOnAkPostEventCallback nullCallback;
	
	if (!CanTakeDamage)
	{
		UAkGameplayStatics::PostEvent(
			HitInvulnerabilitySound,
			this,
			0,
			nullCallback,
			false
		);
		return;
	}
	//GetWorld()->GetSubsystem<UCameraWorldSubsystem>()->ShakeForSeconds(1, 100);

	//GetWorld()->GetSubsystem<UCameraWorldSubsystem>()->CinematicForSeconds(0.2f, GetActorLocation(), 5);

	if (Damage <= 0 || Damage > 4) return;
	
	if (HasShield()) {
		SetShield(GetShield() - 1);

		UAkGameplayStatics::PostEvent(
			HitInvulnerabilitySound,
			this,
			0,
			nullCallback,
			false
		);
		return;
	}

	int lastLife = Life;

	if ((Life == MaxLife && Damage >= 4) || (Life > LifeToFly && Life - Damage <= 0))
	{
		Life = 1;
	}
	else
	{
		Life -= Damage;
		if (Life <= 0) {
			Life = 0;
		}
	}

	if (lastLife > LifeToFly && lastLife - Damage <= LifeToFly) {
		OnPhaseChange();
	}
	
	StateMachine->ChangeState(EBeamCharacterStateID::Projection);

	UAkGameplayStatics::PostEvent(
		HitPowerSoundList[Damage-1],
		this,
		0,
		nullCallback,
		false
	);
	
	// EFFECT HIT
	OnHitBeam();

	OnLifeChange();
	CheckLife();

}

void ABeamCharacter::ResetLife()
{
	Life = MaxLife; 
	//OnLifeChange();
}

bool ABeamCharacter::IsDead() const
{
	if (this == nullptr) return true;
	return Life <= 0;
}

void ABeamCharacter::OnLifeChange_Implementation() {}

void ABeamCharacter::OnPhaseChange_Implementation() {}

void ABeamCharacter::OnDeath()
{
	if (!cantDie)
	{
		StateMachine->ChangeState(EBeamCharacterStateID::Dead);
		StateMachine->SetCanChangeState(false);
		OnDeathEvent.Broadcast(this);
	}
	else CallNoDie();
}

void ABeamCharacter::CheckLife()
{

	if (StateMachine == nullptr) return;

	if (Life > 0 && Life <= LifeToFly) {
		/*if (StateMachine->GetCurrentStateID() != EBeamCharacterStateID::Fly) {
			StateMachine->ChangeState(EBeamCharacterStateID::Fly);
		}*/
	}
	else if (Life > LifeToFly) {
		/*if (StateMachine->GetCurrentStateID() != EBeamCharacterStateID::Idle) {
			StateMachine->ChangeState(EBeamCharacterStateID::Idle);
		}*/
	}
	else {
		OnDeath();
	}
}
#pragma endregion

#pragma region Push

void ABeamCharacter::Push()
{
	WhenPush();

	if (PlayersInZone.Num() == 0 || CharacterSettings == nullptr) return;

	for (ABeamCharacter* player : PlayersInZone) {

		// Parameter for how far out the the line trace reaches
		float Reach = 210.f;
		
		FVector start = GetActorLocation();
		FVector dir = (player->GetActorLocation() - start).GetSafeNormal();
		FVector LineTraceEnd = GetActorLocation() + dir * Reach;

		//DrawDebugLine(GetWorld(), GetActorLocation(), LineTraceEnd, FColor::Green, false, 1, 0, 5);

		FHitResult Hit;
		FCollisionQueryParams TraceParams(FName(TEXT("TraceTest")), true, this);  
		TraceParams.AddIgnoredActor(this);


		bool bHit = GetWorld()->LineTraceSingleByChannel(
			OUT Hit,
			start,
			LineTraceEnd,
			ECC_WorldDynamic,
			TraceParams
		);

		if (bHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
				FString::Printf(TEXT("Hit Component: %s"), *Hit.GetComponent()->GetName()));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
				FString::Printf(TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName()));
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
				FString::Printf(TEXT("NO HIT")));
		}
		

		if (Cast<ABeamCharacter>(Hit.GetActor()) != nullptr && Cast<ABeamCharacter>(Hit.GetActor()) != this) {
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Emerald, FString::Printf(TEXT("PUSH : %d"), player->GetPlayerIndex()));
			FVector direction = (player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			player->KnockBack(direction, CharacterSettings->Push_Force, true);

			const FOnAkPostEventCallback nullCallback;
			UAkGameplayStatics::PostEvent(
					PushContactSound,
					this,
					0,
					nullCallback,
					false
			);
		}

		
	}

}

void ABeamCharacter::SetupCollision()
{
	boxCollision = GetComponentByClass<UBoxComponent>();

	if (boxCollision == nullptr || CharacterSettings == nullptr) return;

	boxCollision->SetBoxExtent(FVector(CharacterSettings->ZoneKnockback_Size.X, 40.f, CharacterSettings->ZoneKnockback_Size.Y));

	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Emerald, FString::Printf(TEXT("WOWWWW : %s"), *boxCollision->GetName()));

	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABeamCharacter::OnBeginOverlapZone);
	boxCollision->OnComponentEndOverlap.AddDynamic(this, &ABeamCharacter::OnEndOverlapZone);

	capsuleCollision = GetComponentByClass<UCapsuleComponent>();

	if (capsuleCollision == nullptr) return;

	capsuleCollision->OnComponentHit.AddDynamic(this, &ABeamCharacter::OnHit);
}

void ABeamCharacter::OnBeginOverlapZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABeamCharacter* player = Cast<ABeamCharacter>(OtherActor);
	if (player == nullptr) return;

	PlayersInZone.Add(player);
}

void ABeamCharacter::OnEndOverlapZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABeamCharacter* player = Cast<ABeamCharacter>(OtherActor);
	if (player == nullptr) return;

	PlayersInZone.Remove(player);
}
#pragma endregion 

#pragma region Stun

bool ABeamCharacter::isShooting()
{

	if (weaponComp == nullptr) return false;
	return weaponComp->GetIsQteActive();
}

void ABeamCharacter::Stun(float TimeToStun = 3.f)
{
	SetStunTime(TimeToStun);

	if (StateMachine != nullptr)
		StateMachine->ChangeState(EBeamCharacterStateID::Stun);

}

void ABeamCharacter::UnFreeze()
{
	if (StateMachine == nullptr) return;

	isFreeze = false;
	StateMachine->SetCanChangeState(true);
	StateMachine->ChangeState(EBeamCharacterStateID::Idle); 
	
}

void ABeamCharacter::Freeze()
{
	if (StateMachine == nullptr) return;

	isFreeze = true;
	Stun(999);
	StateMachine->SetCanChangeState(false);
}

bool ABeamCharacter::IsStunned()
{

	//EBeamCharacterStateID e = StateMachine->GetCurrentStateID();
	//FString s;

	//switch (e) {
	//	case (EBeamCharacterStateID::Idle):
	//	s = "Idle";
	//	break;
	//	case (EBeamCharacterStateID::Walk):
	//	s = "Walk";
	//	break;
	//	case (EBeamCharacterStateID::Jump):
	//		s = "Jump";
	//		break;
	//	case (EBeamCharacterStateID::Fall):
	//		s = "Fall";
	//		break;
	//	case (EBeamCharacterStateID::Push):
	//		s = "Push";
	//		break;
	//	case (EBeamCharacterStateID::Projection):
	//		s = "Projection";
	//		break;
	//	case (EBeamCharacterStateID::Stun):
	//		s = "Stun";
	//		break;
	//	case (EBeamCharacterStateID::Fly):
	//		s = "Fly";
	//		break;
	//	case (EBeamCharacterStateID::Dead):
	//		s = "Dead";
	//		break;
	//	default:
	//		s = "None";
	//		break;
	//}
	//

	if (StateMachine == nullptr) return false;
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Blue, FString::Printf(TEXT("STATE STUNNED : %s"), *s) );
	return StateMachine->GetCurrentStateID() == EBeamCharacterStateID::Stun;
}

#pragma endregion

#pragma region FollowTarget

bool ABeamCharacter::IsFollowable() { return true; }

FVector ABeamCharacter::GetFollowPosition() {return GetActorLocation();}

#pragma endregion

#pragma region Shoot

bool ABeamCharacter::IsChargeReady()
{
	if (playerAimComp == nullptr) return false;

	return playerAimComp->IsReady();
}

bool ABeamCharacter::TraceCheckBeforeProjectile(FVector endPosition, int power)
{
	FVector start = GetActorLocation() + FVector(.0f, .0f, GetCharacterSettings()->AimVerticalOffsetPhase1) + ((endPosition - GetActorLocation()).GetSafeNormal() * (shootRadius[power] - 34.f));
	endPosition = endPosition + ((endPosition - GetActorLocation()).GetSafeNormal() * (2* shootHalfHeight[power])) + FVector(.0f, .0f, GetCharacterSettings()->AimVerticalOffsetPhase1) ;
	
	TArray<FHitResult> hitResults;
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(this);
	
	UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		start,
		endPosition,
		shootRadius[power],
		TraceTypeQuery1,
		false,
		ignoreActors,
		EDrawDebugTrace::None,
		hitResults,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		15.f
	);

	//Pré-tri pour verifier tout les actor valides
	hitResults.RemoveAll([](const FHitResult& hit)
	{
		return hit.GetActor() == nullptr;
	});

	//Tri Croissant de la distance separant les au BeamCharacter
	FVector characterLocation = GetActorLocation();
	hitResults.Sort([&characterLocation](const FHitResult& A, const FHitResult& B)
	{
		const float DistanceA = FVector::DistSquared(A.GetActor()->GetActorLocation(), characterLocation);
		const float DistanceB = FVector::DistSquared(B.GetActor()->GetActorLocation(), characterLocation);

		return DistanceA < DistanceB;
	});

	
	for (FHitResult& hitResult : hitResults)
	{
		FHitResult* HitResultPtr = &hitResult;

		if (HitResultPtr->GetActor()->Implements<UProjectileInterface>())
		{
			IProjectileInterface* interface = Cast<IProjectileInterface>(HitResultPtr->GetActor());
			switch (interface->ProjectileGetType())
			{
			case EProjectileType::DestructWall:
				{
					if (interface->ProjectileContext(power, HitResultPtr->Location))
					{
						ShotDestroyVFX();
						return false;
					}

					break;
				}
			case EProjectileType::Player:
				{
					if (!HitResultPtr->Component->ComponentTags.Contains("Player")) break;

					if (interface->ProjectileContext(power, HitResultPtr->Location))
					{
						ShotDestroyVFX();
						return false;
					}
					
					break;
				}
			case EProjectileType::Bullet:
				{
					TObjectPtr<AProjectile> otherBullet = interface->GetProjectile();
					if(otherBullet == nullptr) break;

					int otherPower = otherBullet->GetPower();

					if (otherPower > power && otherPower >= 3)
					{
						ShotDestroyVFX();
						return false;
					}
					else if (otherPower < power && power >= 3)
					{
						if (otherBullet != nullptr) otherBullet->CallDestroyed();
						return true;
					}
					else
					{
						if (otherBullet != nullptr) otherBullet->CallDestroyed();
						ShotDestroyVFX();
						return false;
					}
				}
			}
		}
	}

	return true;
}

void ABeamCharacter::CallNoDie_Implementation() {}

void ABeamCharacter::ShotCallCharacter(int power) { ShotCallBP(power); }

void ABeamCharacter::ChangeStateWhenQte()
{
	if (StateMachine->GetCurrentStateID() == EBeamCharacterStateID::Stun) return;
	if (!IsPhaseTwo()) { StateMachine->ChangeState(EBeamCharacterStateID::Idle);}
}

void ABeamCharacter::InitWeaponAndAim()
{
	playerAimComp = GetComponentByClass<UPlayerAim>();
	weaponComp = GetComponentByClass<UWeaponCharge>();

	if (playerAimComp == nullptr || weaponComp == nullptr) return;

	playerAimComp->InitCharacter(this);
	weaponComp->InitCharacter(this);
	weaponComp->InitAim(playerAimComp);
	playerAimComp->InitWeapon(weaponComp);

	//localPlayerAim = playerAimComp;

	InitQTE(this);

	if (CharacterSettings == nullptr) return;
	playerAimComp->Radius = CharacterSettings->RadiusShoot;

}
#pragma endregion

#pragma region Character Input

void ABeamCharacter::SetupMappingContextIntoController() const
{
	APlayerController* playerController = Cast<APlayerController>(Controller);
	if (playerController == nullptr) return;

	ULocalPlayer* player = playerController->GetLocalPlayer();
	if (player == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (InputSystem == nullptr) return;

	InputSystem->AddMappingContext(InputMappingContext, 0);
}

void ABeamCharacter::BindInputActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (InputData == nullptr) return;

	if (InputData->InputActionMoveVector2D)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveVector2D,
			ETriggerEvent::Started,
			this,
			&ABeamCharacter::OnInputMove
			);
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveVector2D,
			ETriggerEvent::Started,
			this,
			&ABeamCharacter::OnInputJumpJoystick
			);

		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveVector2D,
			ETriggerEvent::Completed,
			this,
			&ABeamCharacter::OnInputMove
		);

		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveVector2D,
			ETriggerEvent::Triggered,
			this,
			&ABeamCharacter::OnInputMove
		);
	}
	if (InputData->InputActionJump)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionJump,
			ETriggerEvent::Started,
			this,
			&ABeamCharacter::OnInputJump
		);

		EnhancedInputComponent->BindAction(
			InputData->InputActionJump,
			ETriggerEvent::Completed,
			this,
			&ABeamCharacter::OnInputJump
		);
	}
	if (InputData->InputActionDash)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionDash,
			ETriggerEvent::Started,
			this,
			&ABeamCharacter::OnInputDash
		);

		EnhancedInputComponent->BindAction(
			InputData->InputActionDash,
			ETriggerEvent::Completed,
			this,
			&ABeamCharacter::OnInputDash
		);
	}

	if (InputData->InputActionCharge)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionCharge,
			ETriggerEvent::Started,
			this,
			&ABeamCharacter::OnInputCharge
		);

		EnhancedInputComponent->BindAction(
			InputData->InputActionCharge,
			ETriggerEvent::Completed,
			this,
			&ABeamCharacter::OnInputCharge
		);
	}
	if (InputData->InputActionAimVector2D)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionAimVector2D,
			ETriggerEvent::Started,
			this,
			&ABeamCharacter::OnInputAim
		);

		EnhancedInputComponent->BindAction(
			InputData->InputActionAimVector2D,
			ETriggerEvent::Completed,
			this,
			&ABeamCharacter::OnInputAim
		);

		EnhancedInputComponent->BindAction(
			InputData->InputActionAimVector2D,
			ETriggerEvent::Triggered,
			this,
			&ABeamCharacter::OnInputAim
		);
	}
	if (InputData->InputActionShoot)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionShoot,
			ETriggerEvent::Started,
			this,
			&ABeamCharacter::OnInputShoot
		);

		EnhancedInputComponent->BindAction(
			InputData->InputActionShoot,
			ETriggerEvent::Completed,
			this,
			&ABeamCharacter::OnInputShoot
		);
	}

	if (InputData->InputActionPunch)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionPunch,
			ETriggerEvent::Started,
			this,
			&ABeamCharacter::OnInputPush
		);

		EnhancedInputComponent->BindAction(
			InputData->InputActionPunch,
			ETriggerEvent::Completed,
			this,
			&ABeamCharacter::OnInputPush
		);
	}

	if (InputData->InputActionFly)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionFly,
			ETriggerEvent::Started,
			this,
			&ABeamCharacter::OnInputFly
		);

		EnhancedInputComponent->BindAction(
			InputData->InputActionFly,
			ETriggerEvent::Completed,
			this,
			&ABeamCharacter::OnInputFly
		);


	}
}


void ABeamCharacter::OnInputMove(const FInputActionValue& InputActionValue)
{
	InputMove = InputActionValue.Get<FVector2D>();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, FString::Printf(TEXT("move : %s"), *InputMove.ToString()));
}
void ABeamCharacter::OnInputJump(const FInputActionValue& InputActionValue) {InputJump = InputActionValue.Get<bool>();}
void ABeamCharacter::OnInputJumpJoystick(const FInputActionValue& InputActionValue)
{
	if (InputActionValue.Get<FVector2D>().Y >= CharacterSettings->Joystick_Jump_SensibilityY &&
		(InputActionValue.Get<FVector2D>().X < CharacterSettings->Joystick_Jump_SensibilityX && InputActionValue.Get<FVector2D>().X > -CharacterSettings->Joystick_Jump_SensibilityX))
	{
		InputJumpJoystick = true;
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("jump joystick : %f"), InputActionValue.Get<FVector2D>().Y));
	}
}
void ABeamCharacter::OnInputDash(const FInputActionValue& InputActionValue) {InputDash = InputActionValue.Get<bool>();}

void ABeamCharacter::OnInputCharge(const FInputActionValue& InputActionValue) {InputCharge = InputActionValue.Get<bool>();}
void ABeamCharacter::OnInputAim(const FInputActionValue& InputActionValue) {InputAim = InputActionValue.Get<FVector2D>();}
void ABeamCharacter::OnInputShoot(const FInputActionValue& InputActionValue) {InputShoot = InputActionValue.Get<bool>();}

void ABeamCharacter::OnInputPush(const FInputActionValue& InputActionValue) {InputPush = InputActionValue.Get<bool>();}

void ABeamCharacter::OnInputFly(const FInputActionValue& InputActionValue) {InputFly = InputActionValue.Get<bool>();}
#pragma endregion

#pragma region UI

void ABeamCharacter::DisplayQte_Implementation(ABeamCharacter* Character) {}
void ABeamCharacter::HideQte_Implementation(ABeamCharacter* Character) {}
void ABeamCharacter::PassQte_Implementation(ABeamCharacter* Character) {}
void ABeamCharacter::FailQte_Implementation(ABeamCharacter* Character) {}
void ABeamCharacter::InitQTE_Implementation(ABeamCharacter* Character) {}
#pragma endregion

#pragma region VFX

void ABeamCharacter::GunBuildUp_Implementation() {}
#pragma endregion

void ABeamCharacter::WhenPush_Implementation() {}
void ABeamCharacter::ChangeColorToWhite_Implementation() {}
void ABeamCharacter::ChangeColorToNormal_Implementation() {}

void ABeamCharacter::ShotCallBP_Implementation(int power){}
void ABeamCharacter::ShotDestroyVFX_Implementation(){}

void ABeamCharacter::OnHitWallProjection_Implementation(FVector locationHit) {}

void ABeamCharacter::OnHitBeam_Implementation() {}

void ABeamCharacter::OnChargeReady_Implementation() {}

void ABeamCharacter::OnLanding_Implementation() {}

void ABeamCharacter::OnJump_Implementation() {}