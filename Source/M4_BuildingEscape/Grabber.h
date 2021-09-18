// (c) Adam Kellingray-Williamson, 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class M4_BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:
	UGrabber();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float Reach = 100.f;

	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	FVector ReachLocation;

	void GetPhysicsHandle();
	FVector GetReachLocation();
	FHitResult GetFirstPhysicsBodyInReach() const;
	void GrabObject();
	void ReleaseObject();
	void SetupInputComponent();
};
