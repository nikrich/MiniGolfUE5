// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ABasePawn::ABasePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;


}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
}

void ABasePawn::Shoot()
{	
	BaseMesh->AddForce(GetForwardVector() * SpeedMultiplier * GetForwardForce(), TEXT("None"), false);
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetMouseCollision(), 25.f, 20.f, FColor::Red, false);

	auto Force = FMath::Clamp(GetDistance(), 0.f, MaxForce);

	DrawDebugLine(GetWorld(), GetActorLocation(), GetMouseCollision(), FColor::Red, false);
}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &ABasePawn::Shoot);
}

FVector ABasePawn::GetForwardVector() const
{
	DrawDebugSphere(GetWorld(), GetMouseCollision(), 25.f, 20, FColor::Red, false);
	FVector DisplacementVector = (GetMouseCollision() - GetActorLocation()) / GetDistance();
	DisplacementVector.Z = 0; // Do not allow ball to go into the air

	UE_LOG(LogTemp, Warning, TEXT("Impact Result X: %f, Y: %f, Z: %f"), DisplacementVector.X, DisplacementVector.Y, DisplacementVector.Z);
	return DisplacementVector;
}

float ABasePawn::GetForwardForce() const
{
	return FMath::Clamp(GetDistance(), 0.f, MaxForce);
}

float ABasePawn::GetDistance() const
{
	return FVector::Dist(GetActorLocation(), GetMouseCollision());
}

FVector ABasePawn::GetMouseCollision() const
{
	FHitResult HitResult;
	bool MouseHitResult = PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		
	return HitResult.ImpactPoint;
}

APlayerController* ABasePawn::GetPlayerController()
{
	return PlayerController;
}

