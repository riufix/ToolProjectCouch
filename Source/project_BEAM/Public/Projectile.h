// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "ProjectileInterface.h"
#include "ProjectileSettings.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Projectile.generated.h"

class AActor;
class UAkAudioEvent;

USTRUCT(BlueprintType)
struct FProjectileParameters

{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float speed = 100.f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float width = 100.f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float height = 100.f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float lifeSpan = 100000.f;
};


UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Projectile"))
class PROJECT_BEAM_API AProjectile : public AActor, public IProjectileInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();
	UFUNCTION()
	void InitialisePower(int power, ABeamCharacter* character);
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UCapsuleComponent* Capsule;
	
	UPROPERTY(BlueprintReadOnly)
	FString actorParentName;
	
	virtual EProjectileType ProjectileGetType() override;
	virtual AProjectile* GetProjectile() override;
	virtual bool ProjectileContext(int power, FVector position) override;

	UFUNCTION(BlueprintCallable)
	int GetPower() { return ownPower;}
	UFUNCTION(BlueprintCallable)
	void CallDestroyed();
	UFUNCTION(BlueprintCallable)
	void CallFakeDestroy(int power);

	UFUNCTION(Blueprintable)
	FProjectileParameters GetCurrentParam()	{return projectileCurrentParam;	};
	UFUNCTION()
	void InitProjectileSettings();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ReInitialisePower(int power);
	
	/** Call to reinitialise the graphics parameters*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Projectile")
	void InitParameters();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Projectile")
	void DestructionEffect(int power);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UProjectileMovementComponent* projectileComponent;
	
	UPROPERTY(Config, EditAnywhere, Category="Power Parameters") 
	TMap<int, FProjectileParameters> powerParameters;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FProjectileParameters projectileCurrentParam;
	
	UPROPERTY(EditAnywhere)
	FHitResult projectileHitResult;
	
	UPROPERTY(BlueprintReadOnly)
	const UProjectileSettings* ProjectileSettings;

	UPROPERTY(BlueprintReadOnly)
	int ownPower;
	UPROPERTY()
	float currentLifeSpan;

	UFUNCTION(BlueprintCallable)
	void SetCanDoDamage(bool newCanDoDamage) { canDoDamage = newCanDoDamage; }
	UFUNCTION(BlueprintCallable)
	void SetIgnoreProjectile(AProjectile* projectile) { projectileToIgnore = projectile; }

	UFUNCTION(BlueprintCallable)
	bool GetCanDoDamage() { return canDoDamage; }
	UFUNCTION(BlueprintCallable)
	AProjectile* GetIgnoreProjectile() { return projectileToIgnore; }

private:
	UPROPERTY()
	bool canAccess = true;

	UPROPERTY()
	bool canDoDamage = true;
	UPROPERTY()
	TObjectPtr<AProjectile> projectileToIgnore;
	
};
