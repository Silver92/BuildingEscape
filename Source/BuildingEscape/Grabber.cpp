// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#define OUT

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
    FindPhysicsHandleComponent();
    SetupInputComponent();
    
}

void UGrabber::FindPhysicsHandleComponent() {
    // Look for attached Physics Handle
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
    }
}

void UGrabber::SetupInputComponent() {
    // Look for attached Input Component (only appears at run time)
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent) {
        UE_LOG(LogTemp, Warning, TEXT("Input component found"));
        // Bind the input axis
        InputComponent -> BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent -> BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }
    else{
        UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName());
    }
}

void UGrabber::Grab() {
    
    // Try and reach any actors with physics body collision channel set
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent();
    auto ActorHit = HitResult.GetActor();
    
    // If we hit something then attach a physics handle
    if (ActorHit != nullptr) {
        // attach physics handle
        if (!PhysicsHandle) {return; }
        PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), ComponentToGrab->GetOwner()->GetActorRotation());
    }
}

void UGrabber::Release() {
    // TODO release physics handle
    if (!PhysicsHandle) {return; }
    PhysicsHandle -> ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    
    // If physics handle is attached
    if ( PhysicsHandle -> GrabbedComponent) {
        // move the object that we are holding
        PhysicsHandle -> SetTargetLocation(GetReachLineEnd());
    }
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
    
    // Draw a red trace in the world to visualise
//    DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 10.0f);
    
    // Setup query parameters
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    
    // Line-trace (AKA Ray-cast) out to reach distance
    FHitResult Hit;
    GetWorld()->LineTraceSingleByObjectType(
                                            OUT Hit,
                                            GetReachLineStart(),
                                            GetReachLineEnd(),
                                        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                                            TraceParameters
                                            );
    
    return Hit;
}

FVector UGrabber::GetReachLineStart() {
    
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
    
    return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() {
    
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
    
    return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}













