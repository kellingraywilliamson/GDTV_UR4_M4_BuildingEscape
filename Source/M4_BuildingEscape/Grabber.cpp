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

void UGrabber::GetPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandleComponent found on object %s!"), *GetOwner()->GetName())
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::GrabObject);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::ReleaseObject);
	}
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	GetPhysicsHandle();
	SetupInputComponent();
}

FVector UGrabber::GetReachLocation()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewpointRotation);
	return PlayerViewpointLocation + (PlayerViewpointRotation.Vector() * Reach);
}

void UGrabber::GrabObject()
{
	const FHitResult ObjectInReach = GetFirstPhysicsBodyInReach();
	AActor* ObjectHit = ObjectInReach.GetActor();

	if (ObjectHit && PhysicsHandle)
	{
		UPrimitiveComponent* ComponentToGrab = ObjectInReach.GetComponent();
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, ReachLocation);
	}
}

void UGrabber::ReleaseObject()
{
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult HitResult;
	const FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(OUT HitResult,
	                                        PlayerViewpointLocation,
	                                        ReachLocation,
	                                        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
	                                        TraceParams);
	return HitResult;
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ReachLocation = GetReachLocation();
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(ReachLocation);
	}
}
