// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	handle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (handle == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Not Found"));
	}
	
	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (handle == nullptr)
	{
		return;
	}
	if (handle->GetGrabbedComponent() != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		handle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
	//FRotator MyRotation = GetComponentRotation();
	//FString RotationString = MyRotation.ToCompactString();
	//UE_LOG(LogTemp, Display, TEXT("Grabber Rotation: %s"), *RotationString)
	// ...
		//ECC_GameTraceChannel2

	/*FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);

	//Geo sweep
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel2, Sphere);

	if (HasHit)
	{
		FString HitActorName = HitResult.GetActor()->GetActorNameOrLabel();
		UE_LOG(LogTemp, Display, TEXT("Hit actor's name: %s"), * HitActorName);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No actor hit"));
	}*/
}

void UGrabber::Grab()
{
	//UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (handle == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Not Found"));
		return;
	}

	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);

	//Geo sweep
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel2, Sphere);
	
	if (HasHit)
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->WakeAllRigidBodies();
		handle->GrabComponentAtLocationWithRotation(HitComponent,NAME_None,HitResult.ImpactPoint,GetComponentRotation());
	}

	/*
	if (HasHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 10);
		DrawDebugSphere(GetWorld(), HitResult.ImpactNormal, 10, 10, FColor::Red, false, 10);

		FString HitActorName = HitResult.GetActor()->GetActorNameOrLabel();
		UE_LOG(LogTemp, Display, TEXT("Hit actor's name: %s"), *HitActorName);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No actor hit"));
	}*/
}

void UGrabber::Release()
{
	//UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (handle == nullptr)
	{
		return;
	}

	if (handle->GetGrabbedComponent() != nullptr)
	{
		handle->GetGrabbedComponent()->WakeAllRigidBodies();
		handle->ReleaseComponent();
	}

	UE_LOG(LogTemp, Display, TEXT("Released grabber"));
}

