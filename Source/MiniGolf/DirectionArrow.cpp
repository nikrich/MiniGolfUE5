// Fill out your copyright notice in the Description page of Project Settings.


#include "DirectionArrow.h"

// Sets default values
ADirectionArrow::ADirectionArrow()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow Body Mesh"));
	RootComponent = ArrowBodyMesh;

	ArrowHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow Head Mesh"));
	ArrowHeadMesh->AttachToComponent(ArrowBodyMesh, FAttachmentTransformRules::KeepWorldTransform);
}

// Called when the game starts or when spawned
void ADirectionArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADirectionArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADirectionArrow::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

