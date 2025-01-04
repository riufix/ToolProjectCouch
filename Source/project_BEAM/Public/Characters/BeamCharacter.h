// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "BeamCharacterInputdata.h"
#include "InputMappingContext.h"
#include "ProjectileInterface.h"
#include "Camera/CameraFollowTarget.h"
#include "GameFramework/Character.h"

#include "BeamCharacter.generated.h"

class UBeamCharacterStateMachine;
class UBeamCharacterSettings;
class UProjectileSettings;
class UEnhancedInputComponent;
class UBoxComponent;
class UPlayerAim;
class UWeaponCharge;
class UAkAudioEvent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathEvent, ABeamCharacter*, pointeurCharacter);

UCLASS()
class PROJECT_BEAM_API ABeamCharacter : public ACharacter, public IProjectileInterface, public ICameraFollowTarget
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABeamCharacter();
	
	virtual EProjectileType ProjectileGetType() override;
	virtual bool ProjectileContext(int power, FVector position) override;
	virtual AProjectile* GetProjectile() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector StartLocation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void DisableInputsPlayer() { SetActorTickEnabled(false); }
	
	UFUNCTION(BlueprintCallable)
	void ActivateInputsPlayer() { SetActorTickEnabled(true); }

#pragma region Orient

public:
	UFUNCTION()
	float GetOrientX() const {return OrientX;}
	UFUNCTION()
	void SetOrientX(float NewOrientX) {OrientX = NewOrientX;}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FRotator GetRotationMesh() const { return GetMesh()->GetRelativeRotation(); }

protected:
	UPROPERTY(BlueprintReadOnly)
	float OrientX = 1.f;
	UFUNCTION()
	void RotateMeshUsingOrientX() const;

#pragma endregion

# pragma region State Machine

public: 
	UFUNCTION()
	void CreateStateMachine();
	UFUNCTION()
	void InitStateMachine();
	UFUNCTION()
	void TickStateMachine(float DeltaTime) const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UBeamCharacterStateMachine> StateMachine;


# pragma endregion

# pragma region Character Settings

public:
	UFUNCTION()
	void InitCharacterSettings();

	UFUNCTION(BlueprintCallable)
	void ReattributeCharacterSettings();

	UFUNCTION()
	const UBeamCharacterSettings* GetCharacterSettings() const {return CharacterSettings;}

protected:
	UPROPERTY(BlueprintReadOnly)
	const UBeamCharacterSettings* CharacterSettings;
	UPROPERTY(BlueprintReadOnly)
	const UProjectileSettings* ProjectileSettings;

# pragma endregion

# pragma region Fight

public:
	UFUNCTION(BlueprintCallable)
	void KnockBack(FVector Direction, float Force, bool projection = false);

	UFUNCTION(BlueprintCallable)
	void Bounce(FVector Normal);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAkAudioEvent> BouncingSound;

	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent* HitComponent,  // The component that was hit
		AActor* OtherActor,                // The other actor involved in the hit
		UPrimitiveComponent* OtherComp,    // The other actor's component that was hit
		FVector NormalImpulse,             // The force applied to resolve the collision
		const FHitResult& Hit              // Detailed information about the hit)
	);

	UFUNCTION(BlueprintCallable)
	float GetBounciness() const {return Bounciness;}
	UFUNCTION(BlueprintCallable)
	float GetMinSizeVelocity() const {return MinSizeVelocity;}

	UFUNCTION(BlueprintCallable)
	bool GetCanTakeDamage() const {return CanTakeDamage;}

	UFUNCTION(BlueprintCallable)
	void SetCanTakeDamage(bool NewCanTakeDamage) {CanTakeDamage = NewCanTakeDamage;}

	UFUNCTION(BlueprintCallable)
	bool GetCanTakeKnockback() {return CanTakeKnockBack;}

	UFUNCTION(BlueprintCallable)
	void SetCanTakeKnockback(bool NewCanTakeKnockback) {CanTakeKnockBack = NewCanTakeKnockback;}


	UFUNCTION(BlueprintCallable)
	bool GetIsDashing() const {return IsDashing;}


	UFUNCTION(BlueprintCallable)
	void SetIsDashing(bool NewIsDashing) {IsDashing = NewIsDashing;}


private:

	UPROPERTY()
	bool IsDashing = false;

	UPROPERTY()
	float Bounciness = 0.7;

	UPROPERTY()
	float MinSizeVelocity = 100;

	UPROPERTY()
	bool CanTakeDamage = true;

	UPROPERTY()
	bool CanTakeKnockBack = true;

# pragma endregion

# pragma region Life

public:
	// GETTERS
	UFUNCTION(BlueprintCallable)
	int GetLife() const {return Life;}
	UFUNCTION(BlueprintCallable)
	int GetMaxLife() const {return MaxLife;}
	UFUNCTION(BlueprintCallable)
	int GetLifeToFly() const {return LifeToFly;}

	UFUNCTION(BlueprintCallable)
	bool IsDead() const;


	// SETTERS
	UFUNCTION()
	void SetLife(const int NewLife) {Life = NewLife;}
	UFUNCTION()
	void SetMaxLife(const int NewMaxLife) {MaxLife = NewMaxLife;}
	UFUNCTION()
	void SetLifeToFly(const int NewLifeToFly) {LifeToFly = NewLifeToFly;}

	
	// OTHERS
	UFUNCTION(BlueprintCallable)
	void PlayerTakeDamage(const int Damage = 1);
	UFUNCTION(BlueprintCallable)
	void ResetLife();
	UFUNCTION(BlueprintCallable)
	bool IsPhaseTwo() const {return Life <= LifeToFly;}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Call Animation")
	void OnLifeChange();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Call Animation")
	void OnPhaseChange();
	UFUNCTION()
	void OnDeath();

	UFUNCTION(BlueprintCallable)
	bool HasShield() const {return Shield > 0;}
	UFUNCTION()
	void SetShield(int NewShield) {Shield = NewShield;}
	UFUNCTION()
	int GetShield() const {return Shield;}


protected:
	UFUNCTION()
	void CheckLife();

	UPROPERTY(BlueprintReadOnly)
	int Life;
	UPROPERTY(BlueprintReadOnly)
	int MaxLife;
	UPROPERTY(BlueprintReadOnly)
	int LifeToFly;

	UPROPERTY(BlueprintReadOnly)
	int Shield = 0;

# pragma endregion
	
# pragma region Push

public:
	UFUNCTION()
	void Push();
	UFUNCTION()
	bool CanPush() const {return canPush;}

	UFUNCTION()
	void SetCanPush(bool NewCanPush) {canPush = NewCanPush;}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Push Player")
	void WhenPush();

private:
	UPROPERTY()
	UBoxComponent* boxCollision;
	UPROPERTY()
	TArray<ABeamCharacter*> PlayersInZone;

	UPROPERTY()
	UCapsuleComponent* capsuleCollision;
	
	bool canPush = true;
	UPROPERTY()
	float timerPush = 0.0f;
	UPROPERTY()
	float timeToWaitPush = 2.0f;

	UFUNCTION()
	void TickPush(float DeltaTime);
	UFUNCTION()
	void SetupCollision();

	UFUNCTION()
	void OnBeginOverlapZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

	
	UFUNCTION()
	void OnEndOverlapZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public: 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Overlap Zone")
	void OnHitWallProjection(FVector locationHit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Overlap Zone")
	void OnHitBeam();


# pragma endregion

# pragma region Stun

public:
	void Stun(float TimeToStun);
	
	float GetStunTime() const {return StunTime;}
	void SetStunTime(float NewStunTime) {StunTime = NewStunTime;}

	void SetMultiplierStun(float NewMultiplierStun) {MultiplierStun = NewMultiplierStun;}
	float GetMultiplierStun() const {return MultiplierStun;}
	
	UFUNCTION(BlueprintCallable)
	bool GetIsFreeze() { return isFreeze; }
	UFUNCTION(BlueprintCallable)
	void UnFreeze();
	UFUNCTION(BlueprintCallable)
	void Freeze();

	UFUNCTION(BlueprintCallable)
	bool IsStunned();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Push Player")
	void ChangeColorToWhite();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Push Player")
	void ChangeColorToNormal();

private:

	UPROPERTY()
	bool isFreeze = false;
	UPROPERTY()
	float StunTime = 1.f;
	UPROPERTY()
	float MultiplierStun = 0.f;

# pragma endregion

#pragma region FollowTarget

	virtual bool IsFollowable() override;

	virtual FVector GetFollowPosition() override;

#pragma endregion

#pragma region Shoot

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shoot")
	void OnChargeReady();

	UFUNCTION(BlueprintCallable)
	bool IsChargeReady();

	UFUNCTION(BlueprintCallable)
	UPlayerAim* GetPlayerAimComp() const {return playerAimComp;}

	UFUNCTION(BlueprintCallable)
	UWeaponCharge* GetWeaponComp() const {return weaponComp;}

	UFUNCTION(BlueprintCallable)
	bool TraceCheckBeforeProjectile(FVector endPosition, int power);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shoot")
	void ShotDestroyVFX();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Widget QTE")
	void InitQTE(ABeamCharacter* Character);

	UFUNCTION(BlueprintCallable)
	void ChangeStateWhenQte();

	//bool isShooting = false;
	
private:
	UFUNCTION()
	void InitWeaponAndAim();


	UPROPERTY()
	TArray<float> shootRadius;
	UPROPERTY()
	TArray<float> shootHalfHeight;
	
	UPROPERTY()
	UPlayerAim* playerAimComp;
	UPROPERTY()
	UWeaponCharge* weaponComp;

#pragma endregion

# pragma region Character Input

public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY()
	TObjectPtr<UBeamCharacterInputData> InputData;
	

	UFUNCTION() FVector2D GetInputMove() const {return InputMove;}
	UFUNCTION() bool GetInputJump() const {return InputJump;}
	UFUNCTION() bool GetInputJumpJoystick() const {return InputJumpJoystick;}
	UFUNCTION() bool GetInputDash() const {return InputDash;}


	UFUNCTION() bool GetInputCharge() const {return InputCharge;}
	UFUNCTION(BlueprintCallable) FVector2D GetInputAim() const {return InputAim;}
	UFUNCTION() bool GetInputShoot() const {return InputShoot;}

	UFUNCTION() bool GetInputPush() const {return InputPush;}

	UFUNCTION() bool GetInputFly() const {return InputFly;}


protected:
	UFUNCTION()
	void SetupMappingContextIntoController() const;

	UPROPERTY() FVector2D InputMove = FVector2D::ZeroVector;
	UPROPERTY() bool InputJump = false;
	UPROPERTY() bool InputJumpJoystick = false;
	UPROPERTY() bool InputDash = false;

	UPROPERTY() bool InputCharge = false;
	UPROPERTY() FVector2D InputAim = FVector2D::ZeroVector;
	UPROPERTY() bool InputShoot = false;

	UPROPERTY() bool InputPush = false;

	UPROPERTY() bool InputFly = false;

private:
	UFUNCTION()
	void BindInputActions(UEnhancedInputComponent* EnhancedInputComponent);

	UFUNCTION() void OnInputMove(const FInputActionValue& InputActionValue);
	UFUNCTION() void OnInputJump(const FInputActionValue& InputActionValue);
	UFUNCTION() void OnInputJumpJoystick(const FInputActionValue& InputActionValue);
	UFUNCTION() void OnInputDash(const FInputActionValue& InputActionValue);

	UFUNCTION() void OnInputCharge(const FInputActionValue& InputActionValue);
	UFUNCTION() void OnInputAim(const FInputActionValue& InputActionValue);
	UFUNCTION() void OnInputShoot(const FInputActionValue& InputActionValue);

	UFUNCTION() void OnInputPush(const FInputActionValue& InputActionValue);

	UFUNCTION() void OnInputFly(const FInputActionValue& InputActionValue);

# pragma endregion
	
# pragma region Player Aim

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPlayerAim> localPlayerAim;

	UFUNCTION(BlueprintCallable)
	bool isShooting();

private:


	UPROPERTY()
	UBoxComponent* boxAim;

#pragma endregion

#pragma region UI
	public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Widget QTE")
	void DisplayQte(ABeamCharacter* Character);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Widget QTE")
	void HideQte(ABeamCharacter* Character);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Widget QTE")
	void PassQte(ABeamCharacter* Character);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Widget QTE")
	void FailQte(ABeamCharacter* Character);
#pragma endregion

#pragma region DeathEvent

public:
	UPROPERTY(BlueprintAssignable)
	FOnDeathEvent OnDeathEvent;

	UFUNCTION(BlueprintCallable)
	void SetCantDie(bool newValue) { cantDie = newValue; }

protected:
	UPROPERTY(BlueprintReadWrite)
	bool cantDie;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shooting")
	void CallNoDie();

#pragma endregion

#pragma region VFX
	public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player VFX")
	void GunBuildUp();
#pragma endregion

#pragma region SFX

public:
	UFUNCTION()
	void ShotCallCharacter(int power);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shooting")
	void ShotCallBP(int power);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAkAudioEvent> HitInvulnerabilitySound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UAkAudioEvent>> HitPowerSoundList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAkAudioEvent> PushContactSound;
	
#pragma endregion
	
#pragma region Multiplayer

public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerIndex(int NewPlayerIndex) { PlayerIndex = NewPlayerIndex; }
	UFUNCTION(BlueprintCallable)
	int GetPlayerIndex() const { return PlayerIndex; }

private:
	UPROPERTY()
	int PlayerIndex = -1;

#pragma endregion


public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Landing")
	void OnLanding();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Jumping")
	void OnJump();

};
