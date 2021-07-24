// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ACollectable::ACollectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up root component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Set up static mesh component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(Root);

	// Set up collision
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCapsuleSize(30.0f, 70.0f);
	CapsuleComponent->SetupAttachment(Root);

	// Initialize movement variables, this is used for rotating the actor
	PitchValue = 0.f;
	YawValue = 0.8f;
	RollValue = 0.f;

	// Get visual asset
	static ConstructorHelpers::FObjectFinder<UStaticMesh> VisualAsset(TEXT("/Game/MeshAssets/SM_Potion_01.SM_Potion_01"));
	if (VisualAsset.Succeeded())
	{
		// Set this visual asset as the static mesh
		StaticMeshComponent->SetStaticMesh(VisualAsset.Object);

		// Set position
		FVector Pos;
		Pos.X = 0.0f;
		Pos.Y = 0.0f;
		Pos.Z = 0.0f;

		// Set Rotation
		FRotator Rot;
		Rot.Yaw = 0.0f;
		Rot.Pitch = 0.0f;
		Rot.Roll = -10.0f;

		// Set scale
		FVector Scale;
		Scale.X = 3.27;
		Scale.Y = 3.27;
		Scale.Z = 3.27;

		StaticMeshComponent->SetRelativeLocationAndRotation(Pos, Rot);
		StaticMeshComponent->SetRelativeScale3D(Scale);
		StaticMeshComponent->SetWorldScale3D(Scale);

		// Disable collision with static mesh
		StaticMeshComponent->SetCollisionProfileName(TEXT("OverlapAll"));

		// Set collision trigger for capsule component
		CapsuleComponent->SetCollisionProfileName(TEXT("Trigger"));

		// Ensure capsule component's location is correct relative to root
		Pos.Z += 45.0f;
		CapsuleComponent->SetRelativeLocation(Pos);
	}	
	
}

// Called when the game starts or when spawned
void ACollectable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate actor
	FRotator Rotation = FRotator(PitchValue, YawValue, RollValue);
	FQuat QuatRotation = FQuat(Rotation);
	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);	

	// Slowly bob actor up and down
	FVector Location = GetActorLocation();
	Location.Z += 1.12f * FMath::Sin(GetGameTimeSinceCreation() + DeltaTime) - FMath::Sin(GetGameTimeSinceCreation());
	SetActorLocation(Location);
}

