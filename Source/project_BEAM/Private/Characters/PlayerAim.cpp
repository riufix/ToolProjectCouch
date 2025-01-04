// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerAim.h"
#include "Projectile.h"
#include "Characters/BeamCharacter.h"
#include "WeaponCharge.h"
#include "Characters/BeamCharacterSettings.h"


// Sets default values for this component's properties
UPlayerAim::UPlayerAim()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


void UPlayerAim::InitCharacter(ABeamCharacter* playerCharacter)
{
	Character = playerCharacter;
	shootDelayInit = Character->GetCharacterSettings()->ShootCoolDown;
}

void UPlayerAim::InitWeapon(UWeaponCharge* playerWeapon)
{
	if (playerWeapon == nullptr) return;
	Weapon = playerWeapon;
	Weapon->InitValues();
}

void UPlayerAim::ShotCall(int power)
{
	Character->ShotCallCharacter(power);
	Shoot(aimPos, aimDir.GetSafeNormal(), power);
}

FVector UPlayerAim::AimCursorPos(const FVector2D& dir, const FVector& playerPos, const float DeltaTime, float interpSpeed = 10)
{	

	if (interpSpeed == 10) {
		interpSpeed = Character->GetCharacterSettings()->SpeedCursor;
	}

	FVector2D DirNormal = dir.GetSafeNormal();

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("DirNormal : %s"), *DirNormal.ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("DirNormal : %d"), DirNormal.Length()));

	FVector TargetPos = FVector(playerPos.X + DirNormal.X * Radius, playerPos.Y, playerPos.Z + DirNormal.Y * Radius);

	FVector newPos = FMath::VInterpTo(aimPos, TargetPos, DeltaTime, interpSpeed);

	return newPos;


}

void UPlayerAim::Shoot(FVector spawnLocation, FVector2D direction, int power)
{
	if(shootDelay <= 0.f)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Shot Delay good")));
		if(!ProjectileActor)
		{
			GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Red,FString::Printf(TEXT("No ProjectileActor")));
			return;
		}
		
		if (power > 3) power = 3;
		if (power < 0) power = 0;

		FVector newDir = FVector(direction.X, .0f, direction.Y);
		if(newDir == FVector::ZeroVector) newDir = FVector(Character->GetOrientX(), .0f, .0f);
		if(newDir == FVector::ZeroVector) newDir = FVector(1.f, .0f, .0f);
		
		if (Character->TraceCheckBeforeProjectile(spawnLocation, power))
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = Character->GetOwner();
			spawnParams.Instigator = Character->GetInstigator();

			spawnLocation += FVector(.0f, .0f, Character->GetCharacterSettings()->AimVerticalOffsetPhase1); 
			AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileActor, spawnLocation, newDir.ToOrientationRotator(), spawnParams);
			if(projectile == nullptr) return;
		
			projectile->InitialisePower(power, Character);
		}
		//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Blue,FString::Printf(TEXT("shot")));

		if (Character->GetCharacterSettings()->ChargesKnockbacks.Num() <= power) Character->KnockBack(-newDir, 1000.f);
		else Character->KnockBack(-newDir, Character->GetCharacterSettings()->ChargesKnockbacks[power]);
	}
	shootDelay = shootDelayInit;
}

// Called every frame
void UPlayerAim::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	aimPosDuplicate = AimCursorPos(aimDir, Character->GetActorLocation(), DeltaTime);

	if (Character->IsStunned()) {
		//UE_LOG(LogTemp, Error, TEXT("IS STUNNED PLAYER"));
		//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Blue, TEXT("IS STUNNED"));
		return;
	}
	/*else {
		UE_LOG(LogTemp, Error, TEXT("IS NOT STUNNED PLAYER"));
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Blue, TEXT("IS NOT STUNNED"));
	}*/

	aimPos = AimCursorPos(aimDir, Character->GetActorLocation(), DeltaTime);

	if(Character->GetInputShoot() && !Character->IsStunned() && !wasShootTriggered) //check input and if it was recently pressed
	{
		wasShootTriggered = true;
		if(shootDelay <= 0.f) Weapon->StartWeaponCharge(); //activate qte
		
	}
	if(!Character->GetInputShoot() && wasShootTriggered) //no input enter and no recent action
	{
		wasShootTriggered = false;
		
		if (Weapon->GetIsQteActive()) Weapon->CancelWeaponCharge(false); //if qte still active -> deactivate Qte
	}

	// Aim Position
	if (Character->GetInputAim() != FVector2D::ZeroVector)
	{
		aimDir = Character->GetInputAim();
		aimPos = AimCursorPos(aimDir, Character->GetActorLocation(), DeltaTime);

		if (Weapon->GetIsQteActive() && !isAimWhileCharge) isAimWhileCharge = true;
	}
	else if (Character->GetInputMove() != FVector2D::ZeroVector && !isAimWhileCharge)
	{
		aimDir = Character->GetInputMove();

		// Comment if needed, Switch from all dir aim to 4 or 8 dir
		// Comment every marked lines for switching 4dir -> 8dir
		//
		// if (abs(aimDir.X) > abs(aimDir.Y))//
		// {//
			// if (aimDir.X > .5f) aimDir.X = 1.f;
			// else if (aimDir.X < -.5f) aimDir.X = -1.f;
			// else aimDir.X = 0.f;
		
		// 	aimDir.Y = 0.f;//
		// }//
		// else//
		// {//
			// if (aimDir.Y > .5f) aimDir.Y = 1.f;
			// else if (aimDir.Y < -.5f) aimDir.Y = -1.f;
			// else aimDir.Y = 0.f;
		
		// 	aimDir.X = 0.f;//
		// }//
		
		aimPos = AimCursorPos(aimDir, Character->GetActorLocation(), DeltaTime);
	}
	if (!Weapon->GetIsQteActive() && isAimWhileCharge) isAimWhileCharge = false;

	
	if (shootDelay > .0f) { 
		shootDelay -= GetWorld()->GetDeltaSeconds(); 
		if (shootDelay <= .0f && Character != nullptr) {
			Character->OnChargeReady();
		}
	}
}

