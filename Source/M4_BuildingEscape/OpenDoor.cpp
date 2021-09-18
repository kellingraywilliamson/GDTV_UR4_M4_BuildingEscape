// (c) Adam Kellingray-Williamson, 2021


#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

#define  OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing audio component!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no pressure plate attached!"), *GetOwner()->GetName());
	}
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	OpenAngle += InitialYaw;
	CurrentYaw = InitialYaw;

	FindPressurePlate();
	FindAudioComponent();

	DoorLastOpened = GetWorld()->GetTimeSeconds();
}


float UOpenDoor::TotalMassOfActors() const
{
	if (!PressurePlate) { return 0.f; }

	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		TotalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && TotalMassOfActors() > MassRequiredToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::PlayDoorOpenSound()
{
	if (AllowDoorOpenSound && AudioComponent)
	{
		AudioComponent->Play();
		AllowDoorOpenSound = false;
		AllowDoorCloseSound = true;
	}
}

void UOpenDoor::OpenDoor(const float DeltaTime)
{
	CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed);
	GetOwner()->SetActorRotation(DoorRotation);

	PlayDoorOpenSound();
}

void UOpenDoor::PlayDoorCloseSound()
{
	if (AllowDoorCloseSound && AudioComponent)
	{
		AudioComponent->Play();
		AllowDoorOpenSound = true;
		AllowDoorCloseSound = false;
	}
}

void UOpenDoor::CloseDoor(const float DeltaTime)
{
	CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	GetOwner()->SetActorRotation(DoorRotation);

	PlayDoorCloseSound();
}
