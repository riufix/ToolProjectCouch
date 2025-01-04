// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Characters/BeamCharacter.h"
#include <Camera/CameraWorldSubsystem.h>


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	projectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	Capsule->SetCollisionProfileName(TEXT("OverlapAll"));
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	projectileComponent->ProjectileGravityScale = 0.f;
	currentLifeSpan = 0.f;
	
	Capsule->IgnoreActorWhenMoving(this, true);
	Capsule->IgnoreActorWhenMoving(GetOwner(), true);

	InitProjectileSettings();
}

void AProjectile::InitialisePower(int power, ABeamCharacter* character)
{
	ownPower = power;
	projectileCurrentParam = powerParameters[power];
	
	Capsule->IgnoreActorWhenMoving(character, true);
	actorParentName = character->GetName();
	
	InitParameters();
	
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
void AProjectile::ReInitialisePower(int power)
{
	ownPower = power;
	projectileCurrentParam = powerParameters[power];
	
	InitParameters();
}


void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (actorParentName == OtherActor->GetName()) return;
	
	if(OtherActor && OtherActor != this) //check if actor is not null
	{
		GetWorld()->GetSubsystem<UCameraWorldSubsystem>()->ShakeForSeconds(ownPower * 0.2,(ownPower^2) * 10);

		 // disable collider to detected self
		if(OtherActor->Implements<UProjectileInterface>())
		{
			IProjectileInterface* interface = Cast<IProjectileInterface>(OtherActor);
			if (interface == nullptr) return;
			

			switch (interface->ProjectileGetType())
			{
			case EProjectileType::Player:
				{
					if (!OtherComp->ComponentTags.Contains("Player")) break;
					
					if (!canDoDamage) break;

					if (interface->ProjectileContext(ownPower, GetActorLocation())) { 
						CallDestroyed(); 
						SetCanDoDamage(false);
					}
					else return;

					break;
				};
			
			case EProjectileType::Bullet:
				{
					if (!canAccess) break;
					
					TObjectPtr<AProjectile> otherBullet = interface->GetProjectile();
					if (otherBullet == nullptr) break;

					if (projectileToIgnore == otherBullet || otherBullet->GetIgnoreProjectile() == this) break;
					
					int otherPower = otherBullet->GetPower();
					
					if (otherPower > ownPower && otherPower >= 3)
					{
						if (otherBullet != nullptr)
						{
							otherBullet->SetIgnoreProjectile(this);
						}
						projectileToIgnore = otherBullet;
						CallDestroyed();
					}
					else if (otherPower < ownPower && ownPower >= 3)
					{
						if (otherBullet != nullptr)
						{
							otherBullet->SetIgnoreProjectile(this);
							otherBullet->CallDestroyed();
						}
						projectileToIgnore = otherBullet;
					}
					else
					{
						if (otherBullet != nullptr)
						{
							otherBullet->SetIgnoreProjectile(this);
							otherBullet->CallDestroyed();
						}
						projectileToIgnore = otherBullet;
						CallDestroyed();
					}

					break;
				};

			case EProjectileType::DestructWall:
				{
					if (interface->ProjectileContext(ownPower, GetActorLocation())) CallDestroyed();

					break;
				};
			}
		}
		else //if actor doesn't implement interface
		{
			CallDestroyed();
		}
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	currentLifeSpan += DeltaTime * projectileCurrentParam.speed;
	if (currentLifeSpan >= projectileCurrentParam.lifeSpan) CallDestroyed();
}



EProjectileType AProjectile::ProjectileGetType()
{
	return EProjectileType::Bullet;
}
AProjectile* AProjectile::GetProjectile()
{
	canAccess = false;
	return this;
}
bool AProjectile::ProjectileContext(int power, FVector position) // Should not be called in any circumstances
{
	return false;
}

void AProjectile::CallDestroyed() // Destroy the projectile
{
	DestructionEffect(ownPower);
	if (ownPower < 3) {
		this->Destroy();
	}

// 	UAkGameplayStatics::PostEventAtLocation(
// 		WwiseEventTest,
// 		this->GetActorLocation(),
// 		FRotator::ZeroRotator,
// 		
// 	);
}
void AProjectile::CallFakeDestroy(int power) // Produce a destruction effect and reset the projectile parameters, does not destroy the Actor
{
	DestructionEffect(power);
	canAccess = true;
	ReInitialisePower(power);
}

void AProjectile::InitProjectileSettings()
{
	ProjectileSettings = GetDefault<UProjectileSettings>();
	if (ProjectileSettings == nullptr) return;
	
	powerParameters.Add( 0, FProjectileParameters(ProjectileSettings->speed_0, ProjectileSettings->width_0, ProjectileSettings->height_0, ProjectileSettings->lifespan_0));
	powerParameters.Add( 1, FProjectileParameters(ProjectileSettings->speed_1, ProjectileSettings->width_1, ProjectileSettings->height_1, ProjectileSettings->lifespan_1));
	powerParameters.Add( 2, FProjectileParameters(ProjectileSettings->speed_2, ProjectileSettings->width_2, ProjectileSettings->height_2, ProjectileSettings->lifespan_2));
	powerParameters.Add( 3, FProjectileParameters(ProjectileSettings->speed_3, ProjectileSettings->width_3, ProjectileSettings->height_3, ProjectileSettings->lifespan_3));
}

void AProjectile::InitParameters_Implementation() {}
void AProjectile::DestructionEffect_Implementation(int power) {}



