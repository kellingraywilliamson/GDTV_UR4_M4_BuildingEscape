// (c) Adam Kellingray-Williamson, 2021

#include "Grabber.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"

#define  OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// Check for physics handle component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandleComponent found on object %s!"), *GetOwner()->GetName())
	}

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Display, TEXT("Grabber pressed!"));
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Grabber released!"));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player viewpoint
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewpointRotation);

	// Log out to test
	// UE_LOG(LogTemp, Display, TEXT("Location: %s, Rotation: %s"), *PlayerViewpointLocation.ToString(), *PlayerViewpointRotation.ToString());

	// Draw a line from player showing their reach
	const FVector LineTraceEnd = PlayerViewpointLocation + (PlayerViewpointRotation.Vector() * Reach);
	DrawDebugLine(GetWorld(),
	              PlayerViewpointLocation,
	              LineTraceEnd,
	              FColor(0, 255, 0),
	              false,
	              0.f,
	              0,
	              5);
	// Raycast out to a certain distance
	FHitResult Hit;
	FCollisionQueryParams TraceParams(
		FName(TEXT("")),
		false,
		GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT Hit,
	                                        PlayerViewpointLocation,
	                                        LineTraceEnd,
	                                        FCollisionObjectQueryParams(
		                                        ECollisionChannel::ECC_PhysicsBody),
	                                        TraceParams);


	AActor* ActorHit = Hit.GetActor();
	// See what it hits
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s was hit!"), *ActorHit->GetName())
	}
}
