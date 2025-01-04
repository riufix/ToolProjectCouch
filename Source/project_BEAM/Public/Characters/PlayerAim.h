// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponCharge.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"

#include "PlayerAim.generated.h"

class ABeamCharacter;
class AProjectile;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_BEAM_API UPlayerAim : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerAim();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void InitCharacter(ABeamCharacter* playerCharacter);
	UFUNCTION()
	void InitWeapon(UWeaponCharge* playerWeapon);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Radius = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABeamCharacter* Character;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWeaponCharge> Weapon;

	UFUNCTION()
	void ShotCall(int power);
	
	UFUNCTION(BlueprintCallable)
	bool GetIsActive() const {return wasShootTriggered;}

	UFUNCTION(BlueprintCallable)
	FVector GetAimPos() {return aimPos;}

	UFUNCTION(BlueprintCallable)
	bool IsReady() { return shootDelay <= .0f;  }
	
protected:
	// Called when the game starts
	
	UFUNCTION(BlueprintCallable)
	FVector AimCursorPos(const FVector2D& dir, const FVector& playerPos, const float DeltaTime, float interpSpeed);
	UFUNCTION(BlueprintCallable)
	void Shoot(FVector spawnLocation, FVector2D direction, int power);
	UFUNCTION(BlueprintCallable)
	float GetShootDelay()
	{
		if (shootDelay < .0f) return .0f;
		return shootDelay;
	}
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileActor;
	UPROPERTY(BlueprintReadOnly)
	FVector aimPos;

	UPROPERTY(BlueprintReadOnly)
	FVector aimPosDuplicate;

	UPROPERTY()
	FVector2D aimDir;
	UPROPERTY()
	bool isAimWhileCharge;
	
private:
	UPROPERTY(EditAnywhere)
	float shootDelay = 0.f;
	UPROPERTY(EditAnywhere)
	float shootDelayInit = 3.f;

	UPROPERTY()
	bool wasShootTriggered = false;
	
};
