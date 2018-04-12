// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UOpenDoor::OpenDoor() {
    
    // Set the door rotation
    Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor() {
    
    // Set the door rotation
    Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
    
    Owner = GetOwner();
    ActorThatOpens = GetWorld() -> GetFirstPlayerController() -> GetPawn();
    
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
    if(PressurePlate->IsOverlappingActor(ActorThatOpens)) {
        OpenDoor();
        LastDoorOpenTime = GetWorld() -> GetTimeSeconds();
    }
    
    // Check if it's time to close the door
    if(GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
        CloseDoor();
    }
    
    
    
}


