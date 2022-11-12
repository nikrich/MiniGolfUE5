// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Projectile.h"
#include "GolfHole.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GolfPlayerController.h"

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

	PlayerController = Cast<AGolfPlayerController>(GetController());
	GolfHole = UGameplayStatics::GetActorOfClass(this, AGolfHole::StaticClass());
}

void ABasePawn::Shoot()
{
	if (!PlayerController) return;

	if (!PlayerController->GetPlayerEnabledState())
		return;

	PlayerController->Shoot();

	BaseMesh->AddImpulse(GetForwardVector() * SpeedMultiplier * GetForwardForce(), TEXT("None"), false);

	if (ShotCameraShake && PlayerController) {
		PlayerController->ClientStartCameraShake(ShotCameraShake);
	}

	if (HitSound) {
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetMouseCollision(), 5.f, 20.f, FColor::Red, false);

	auto Force = FMath::Clamp(GetDistance(), 0.f, MaxForce);

	DrawDebugLine(GetWorld(), GetActorLocation(), GetMouseCollision(), FColor::Red, false);

	StopTurnIfBallStops();
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

	return DisplacementVector;
}

void ABasePawn::StopTurnIfBallStops()
{
	if (BaseMesh->GetComponentVelocity() == FVector::ZeroVector && PlayerController && !PlayerController->GetPlayerEnabledState()) {
		PlayerController->SetPlayerEnabledState(true);

		UE_LOG(LogTemp, Warning, TEXT("Turn ended"));

		if (GolfHole) {
			auto Dist = FVector::DistSquaredXY(BaseMesh->GetComponentLocation(), GolfHole->GetActorLocation());
			UE_LOG(LogTemp, Warning, TEXT("Ball - X: %f, Y: %f, Z: %f"), BaseMesh->GetComponentLocation().X, BaseMesh->GetComponentLocation().Y, BaseMesh->GetComponentLocation().Z);
			UE_LOG(LogTemp, Warning, TEXT("Hole - X: %f, Y: %f, Z: %f"), GolfHole->GetActorLocation().X, GolfHole->GetActorLocation().Y, GolfHole->GetActorLocation().Z);
			UE_LOG(LogTemp, Warning, TEXT("Distance from hole: %f"), (float) Dist);
		}
	}
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
	if (!PlayerController) return FVector::ZeroVector;

	FHitResult HitResult;
	bool MouseHitResult = PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		
	return HitResult.ImpactPoint;
}

APlayerController* ABasePawn::GetPlayerController()
{
	return PlayerController;
}

