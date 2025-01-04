// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "BeamCharacterSettings.generated.h"

class UBeamCharacterInputData;
class UInputMappingContext;

UCLASS(Config=Game, DefaultConfig, meta =(DisplayName = "Beam Character Settings"))
class PROJECT_BEAM_API UBeamCharacterSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static const UBeamCharacterSettings* GetBeamCharacterSettings() { return GetDefault<UBeamCharacterSettings>(); }

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category="Inputs")
	TSoftObjectPtr<UBeamCharacterInputData> InputData;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category="Inputs")
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;

#pragma region General 

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "General", meta = (ToolTip = "Vie max du personnage"))
	int MaxLife;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "General", meta = (ToolTip = "Vie pour aller a la phase 2"))
	int LifeToFly;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "General", meta = (ToolTip = "Force du knockback quand passe en phase 2"))
	float knockbackForce_cinematic;

	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "General", meta = (ToolTip = "Permet de ralentir les deplacements, valeur haute -> ralenti enormement, valeur petite -> ralenti un peu"))
	double BrakingFrictionFactor;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "General", meta = (ToolTip = "L'acceleration, grande -> Va tres vite a sa vitesse max, petite -> va accelerer beaucoup plus lentement pour aller a la vitesse max"))
	double MaxAcceleration;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "General", meta = (ToolTip = "Permet de rendre comme une patinoire ou l'inverse"))
	double GroundFriction;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "General", meta = (ToolTip = "Pour jouer sur le fait de retomber plus vite"))
	double GravityScale;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "General", meta = (ToolTip = "La masse du character :O"))
	double Mass;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "General", meta = (ToolTip = "La valeur minimale pour que le minimum en velocity en Z soit de X pour stun avec la chute"))
	float MinVelocityZStunFall;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "General", meta = (ToolTip = "Multiplier pour le stun avec la chute"))
	float MultiplyerStunFall;

#pragma endregion

#pragma region Walk

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category="Walk", meta = (ToolTip = "La vitesse maximale que le character peut avoir en etat de marche"))
	double Walk_VelocityMax;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Walk", meta = (ToolTip = "Permet de stopper net avec une grande valeur"))
	double BreakingDecelerationWalking;


#pragma endregion

#pragma region Jump
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Jump", meta = (ToolTip = "Pour le deplacement dans les airs"))
	double Jump_VelocityMax;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Jump", meta = (ToolTip = "Force a laquelle le joueur est projet�"))
	double Jump_Force;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Jump", meta = (ToolTip = "Sensibilite du saut via l'input joystick vers le haut"))
	float  Joystick_Jump_SensibilityY;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Jump", meta = (ToolTip = "Sensibilite qui ignore le saut si le move X est trop fort"))
	float  Joystick_Jump_SensibilityX;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Jump", meta = (ToolTip = "Bas -> Pas beaucoup bouger, Haut -> Bouge comme si il etait au sol mais dans les airs"))
	double AirControl;

#pragma endregion

#pragma region Falling
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Fall", meta = (ToolTip = "Friction pour quand il tombe (ralentir la chute)"))
	double FallingLateralFriction;

#pragma endregion

#pragma region Flying

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Fly", meta = (ToolTip = "Vitesse � laquelle il se deplace en flying"))
	double Fly_MaxSpeed;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Fly", meta = (ToolTip = "Permet de ralentir les deplacements, valeur haute -> ralenti enormement, valeur petite -> ralenti un peu"))
	double Fly_BrakingFrictionFactor;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Fly", meta = (ToolTip = "Force du dash (faut mettre une tr�s grande force, genre 100000)"))
	double Fly_DashForce;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Fly", meta = (ToolTip = "Timer du dash"))
	float Fly_DashTimer;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Fly", meta = (ToolTip = "Timer pour remettre les inputs"))
	float Fly_InputsTimer;

#pragma endregion

#pragma region KnockBack

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "KnockBack", meta = (ToolTip = "Force pour le push"))
	float Push_Force;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "KnockBack", meta = (ToolTip = "Temps pour freeze le joueur"))
	float Push_WaitTime;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "KnockBack", meta = (ToolTip = "Temps de cooldown pour le push"))
	float Push_Cooldown;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "KnockBack", meta = (ToolTip = "Force de Knockback des tirs"))
	TArray<float> ChargesKnockbacks;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "KnockBack", meta = (ToolTip = "Force de Knockback des projectils recu"))
	TArray<float> DamageKnockbacks;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "KnockBack", meta = (ToolTip = "Taille de la zone"))
	FVector2D ZoneKnockback_Size;

#pragma endregion

#pragma region Coyote
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Coyote", meta = (ToolTip = "Temps pour le coyote jump"))
	float CoyoteTime;

#pragma endregion

#pragma region Projection

	UPROPERTY(Config, EditAnywhere, Category = "Projection", meta = (ToolTip = "La bounciness pour la projection"))
	double Bounciness;

	UPROPERTY(Config, EditAnywhere, Category = "Projection", meta = (ToolTip = "La taille du vector pour que la projection s'arrete"))
	double MinSizeVelocity;

	UPROPERTY(Config, EditAnywhere, Category = "Projection", meta = (ToolTip = "Temps apres la projection pour se relever"))
	double TimeToWaitAfterProjection;

#pragma endregion
	
#pragma region QTE
public:
	UPROPERTY(Config, EditAnywhere, Category = "QTE", meta = (ToolTip = "Temps maximal du QTE"))
	float QTEMaxTime;

	UPROPERTY(Config, EditAnywhere, Category = "QTE", meta = (ToolTip = "fenetre de tir entre chaque niveaux (Phase 1)"));
	TArray<float> QTETimeStamp;
	UPROPERTY(Config, EditAnywhere, Category = "QTE", meta = (ToolTip = "fenetre de tir entre chaque niveaux (Phase 2)"));
	float QTETimeStampPhase2;

	UPROPERTY(Config, EditAnywhere, Category = "QTE", meta = (ToolTip = "temps d'attente après dernier chargement"));
	float QTELastWait;
	
#pragma endregion

#pragma region Aim

public:
	UPROPERTY(Config, EditAnywhere, Category = "Aim", meta = (ToolTip = "Cooldown entre chaque tir"))
	float ShootCoolDown;
	
	UPROPERTY(Config, EditAnywhere, Category = "Aim", meta = (ToolTip = "Vitesse du curseur"));
	float SpeedCursor;

	UPROPERTY(Config, EditAnywhere, Category = "General", meta = (ToolTip = "Radius pour le tir autour du joueur"));
	float RadiusShoot;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Aim", meta = (ToolTip = "Offset Vertical de la position du spawn du projectile"));
	float AimVerticalOffsetPhase1;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Aim", meta = (ToolTip = "Offset Vertical de la position du spawn du projectile"));
	float AimVerticalOffsetPhase2;

#pragma endregion

#pragma region Charge

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Charge", meta = (ToolTip = "SpawnRate du vfx à la charge 0"));
	float Charge0_SpawnRate;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Charge", meta = (ToolTip = "taille du vfx à la charge 0"));
	float Charge0_Size;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Charge", meta = (ToolTip = "couleur du vfx à la charge 0"));
	FColor Charge0_Color;


	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Charge", meta = (ToolTip = "SpawnRate du vfx à la charge 1"));
	float Charge1_SpawnRate;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Charge", meta = (ToolTip = "taille du vfx à la charge 1"));
	float Charge1_Size;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Charge", meta = (ToolTip = "couleur du vfx à la charge 1"));
	FColor Charge1_Color;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Charge", meta = (ToolTip = "SpawnRate du vfx à la charge 2"));
	float Charge2_SpawnRate;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Charge", meta = (ToolTip = "taille du vfx à la charge 2"));
	float Charge2_Size;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Charge", meta = (ToolTip = "couleur du vfx à la charge 2"));
	FColor Charge2_Color;

#pragma endregion

#pragma region TimeStop
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "TimeStop", meta = (ToolTip = "Speed de l'interpolation du timestop quand un tir est pres du joueur"));
	float TimeStop_Speed;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "TimeStop", meta = (ToolTip = "Valeur minimale pour le timestop quand un tir est pres du joueur"));
	float TimeStop_min;

#pragma endregion

};
