// Fill out your copyright notice in the Description page of Project Settings.

#include "DodgeballProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TopDownCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADodgeballProjectile::ADodgeballProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereComponent->SetSphereRadius(35.f);
	SphereComponent->SetCollisionProfileName(FName("Dodgeball"));
	SphereComponent->SetSimulatePhysics(true);
	//Simulate generate hit event
	SphereComponent->SetNotifyRigidBodyCollision(true);

	// Listen to the OnComponentHit event by binding it to 
	// our function
	SphereComponent->OnComponentHit.AddDynamic(this, &ADodgeballProjectile::OnHit);

	// Set this Sphere Component as the root component,
	// otherwise collision won't behave properly
	RootComponent = SphereComponent;

	ProjectileMovement =
		CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	ProjectileMovement->InitialSpeed = 1500.f;

}

// Called when the game starts or when spawned
void ADodgeballProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(5.f);
}

// Called every frame
void ADodgeballProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADodgeballProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (BounceSound != nullptr && NormalImpulse.Size() > 100.0f)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BounceSound, GetActorLocation(), 1.0f, FMath::RandRange(.7f, 1.3f), 0.0f, BounceSoundAttenuation);
	}

	ATopDownCharacter* Player = Cast<ATopDownCharacter>(OtherActor);
	if (Player != nullptr)
	{
		if (HitParticles != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, GetActorTransform());
		}
		if (DamageSound != nullptr && NormalImpulse.Size() > 100.0f)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DamageSound, GetActorLocation(), 1.0f, FMath::RandRange(.7f, 1.3f), 0.0f, BounceSoundAttenuation);
		}

		Destroy();
	}
}

