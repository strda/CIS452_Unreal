// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "DodgeballProjectile.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyCharacter::ThrowDodgeball()
{
	if (DodgeballClass == nullptr)
	{
		return;
	}

	FVector ForwardVector = GetActorForwardVector();
	float SpawnDistance = 40.f;
	FVector SpawnLocation = GetActorLocation() + (ForwardVector* SpawnDistance);

	//Spawn new dodgeball
	GetWorld()->SpawnActor<ADodgeballProjectile>(DodgeballClass,
		SpawnLocation, GetActorRotation());

}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Fetch character currently controller by player
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	
	// Look at the player character every frame
	bCanSeePlayer = LookAtActor(PlayerCharacter);

	if (bCanSeePlayer != bPreviousCanSeePlayer)
	{
		if (bCanSeePlayer)
		{
			//Start throwing dodgeballs
			GetWorldTimerManager().SetTimer(ThrowTimerHandle, this,
				&AEnemyCharacter::ThrowDodgeball, ThrowingInterval, true, ThrowingDelay);
		}
		else
		{
			//Stop throwing dodgeballs
			GetWorldTimerManager().ClearTimer(ThrowTimerHandle);
		}
	}

	bPreviousCanSeePlayer = bCanSeePlayer;
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AEnemyCharacter::LookAtActor(AActor* TargetActor)
{
	if (TargetActor == nullptr) return false;

	if (CanSeeActor(TargetActor))
	{
		FVector Start = GetActorLocation();
		FVector End = TargetActor->GetActorLocation();
		//Calculation neccesary rotation for start point to face end point
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

		//Set enemy's rotation to that rotation
		SetActorRotation(LookAtRotation);
		return true;
	}
	return false;
}

bool AEnemyCharacter::CanSeeActor(const AActor* const TargetActor)
{
	if (TargetActor == nullptr) return false;

	// Store results of line trace
	FHitResult Hit;

	//Where line trace starts and ends
	FVector Start = GetActorLocation();
	FVector End = TargetActor->GetActorLocation();

	//The trace channel we want to compare against
	ECollisionChannel Channel = ECollisionChannel::ECC_Visibility;

	FCollisionQueryParams QueryParams;
	//Ignor the actor that's executing line trace
	QueryParams.AddIgnoredActor(this);
	//And the target checking for
	QueryParams.AddIgnoredActor(TargetActor);

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, Channel, QueryParams);

	FColor LineColor = (Hit.bBlockingHit) ? FColor::Green : FColor::Red;

	DrawDebugLine(GetWorld(), Start, End, LineColor);

	return !Hit.bBlockingHit;
}

