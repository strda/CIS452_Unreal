// Fill out your copyright notice in the Description page of Project Settings.


#include "OwnershipTestActor.h"
#include "TestMultiplayer.h"
#include "TestMultiplayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AOwnershipTestActor::AOwnershipTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	bReplicates = true;

}

// Called when the game starts or when spawned
void AOwnershipTestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOwnershipTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//draw a debug sphere to help visualize ownership radius
	DrawDebugSphere(GetWorld(),GetActorLocation(), 
	OwnershipRadius, 32, FColor::Yellow);

	//gets closest ownership caharact within radius
	//if its dif from current set as owner
	if (HasAuthority())
	{
		AActor* NextOwner = nullptr;
		float MinDistance = OwnershipRadius;
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(this,
			ATestMultiplayerCharacter::StaticClass(), Actors);
		for (AActor* Actor : Actors)
		{
			const float Distance = GetDistanceTo(Actor);
			if (Distance <= MinDistance)
			{
				MinDistance = Distance;
				NextOwner = Actor;
			}
		}
		if (GetOwner() != NextOwner)
		{
			SetOwner(NextOwner);
		}
	}

	const FString LocalRoleString = ROLE_TO_STRING(GetLocalRole());
	const FString RemoteRoleString = ROLE_TO_STRING(GetRemoteRole());
	const FString OwnerString = GetOwner() != nullptr ? GetOwner()->GetName() : TEXT("No Owner");
	const FString ConnectionString = GetNetConnection() != nullptr ? TEXT("Valid Connection") : TEXT("Invalid Connection");

	const FString Values =
		FString::Printf(TEXT("[Jason D. Shull]\nLocalRole = %s\nRemoteRole = %s\nOwner = %s\nConnection = %s"),
			*LocalRoleString, *RemoteRoleString, *OwnerString, *ConnectionString);
	DrawDebugString(GetWorld(), GetActorLocation(), Values, nullptr, FColor::White, 0.0f, true);
}

