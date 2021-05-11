// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	StartingYaw = GetOwner()->GetActorRotation().Yaw;
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor '%s' has no Pressure Plate"), *GetOwner()->GetName());
	}
	
	ActorThatTriggers = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatTriggers))
		{
			OpenDoor(DeltaTime);
			DoorLastOpened = GetWorld()->GetTimeSeconds();
		}
	else if (PressurePlate && GetWorld()->GetTimeSeconds() - DoorLastOpened > 0.2f)
		CloseDoor(DeltaTime);
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	float CurrentYaw = CurrentRotation.Yaw;

	UE_LOG(LogTemp, Display, TEXT("Current Yaw is: %f"), CurrentYaw);

	float NewYaw = FMath::Lerp(CurrentYaw, StartingYaw + 90.f, DeltaTime*0.5f);
	FRotator NewRotation = CurrentRotation;
	NewRotation.Yaw = NewYaw;
	GetOwner()->SetActorRotation(NewRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	float CurrentYaw = CurrentRotation.Yaw;

	float NewYaw = FMath::Lerp(CurrentYaw, StartingYaw, DeltaTime*5.f);
	FRotator NewRotation = CurrentRotation;
	NewRotation.Yaw = NewYaw;
	GetOwner()->SetActorRotation(NewRotation);
}


