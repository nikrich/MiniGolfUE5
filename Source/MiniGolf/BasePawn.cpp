// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "GolfHole.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GolfPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

// Sets default values
ABasePawn::ABasePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;

	ArrowLengthSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arrow Length Spring Arm"));
	ArrowLengthSpringArm->SetupAttachment(BaseMesh);

	ArrowHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow Head Mesh"));
	ArrowHeadMesh->SetupAttachment(ArrowLengthSpringArm);

	ArrowBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow Body Mesh"));
	ArrowBodyMesh->SetupAttachment(ArrowLengthSpringArm);
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AGolfPlayerController>(GetController());
	GolfHole = UGameplayStatics::GetActorOfClass(this, AGolfHole::StaticClass());

	// Set Arrow Materials
	UMaterialInterface* Material = ArrowBodyMesh->GetMaterial(0);

	if (!Material) return;

	ArrowMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
	ArrowBodyMesh->SetMaterial(0, ArrowMaterial);
	ArrowHeadMesh->SetMaterial(0, ArrowMaterial);
}

void ABasePawn::Shoot()
{
	if (!PlayerController) return;

	if (!CanShoot)
		return;

	CanShoot = false;
	PlayerController->Shoot();


	UE_LOG(LogTemp, Warning, TEXT("Delta: %f"), UGameplayStatics::GetWorldDeltaSeconds(this));
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
	auto Force = FMath::Clamp(GetDistance(), 0.f, MaxForce);
	StopTurnIfBallStops();
	UpdateArrow();
}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &ABasePawn::Shoot);
}

FVector ABasePawn::GetForwardVector() const
{
	FVector DisplacementVector = (GetMouseCollision() - GetActorLocation()) / GetDistance();
	DisplacementVector.Z = 0; // Do not allow ball to go into the air

	return DisplacementVector;
}

void ABasePawn::StopTurnIfBallStops()
{
	if (!PlayerController)
		return;

	if (!PlayerController->GetPlayerEnabledState())
		return;

	auto XSpeed = FMath::Abs(BaseMesh->GetComponentVelocity().X);
	auto YSpeed = FMath::Abs(BaseMesh->GetComponentVelocity().X);

	if ((XSpeed < 0.5f && XSpeed > 0.1f) &&
		(YSpeed < 0.5f && YSpeed > 0.1f) &&
		!CanShoot) {

		BaseMesh->SetPhysicsLinearVelocity(FVector::Zero(), false);
		CanShoot = true;

		UE_LOG(LogTemp, Warning, TEXT("Turn ended"));

		if (GolfHole) {
			auto Dist = FVector::DistSquaredXY(BaseMesh->GetComponentLocation(), GolfHole->GetActorLocation());
			UE_LOG(LogTemp, Warning, TEXT("Distance from hole: %f m"), (float)Dist / 10000);
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

void ABasePawn::UpdateArrow()
{
	if (CanShoot) {
		ArrowLengthSpringArm->SetVisibility(true, true);
	}
	else {
		ArrowLengthSpringArm->SetVisibility(false, true);
	}

	FRotator LookAtRotation = FRotator(0.f, 180 + GetForwardVector().Rotation().Yaw, 0.f);
	ArrowLengthSpringArm->SetWorldRotation(FMath::RInterpTo(ArrowLengthSpringArm->GetComponentRotation(), LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 20.f));
	ArrowLengthSpringArm->TargetArmLength = GetForwardForce();

	FVector BodyScale = ArrowBodyMesh->GetRelativeScale3D();
	BodyScale.X = GetForwardForce() / 105;
	ArrowBodyMesh->SetRelativeScale3D(BodyScale);
	ArrowMaterial->SetScalarParameterValue(TEXT("Strength"), GetForwardForce() / MaxForce);
}

APlayerController* ABasePawn::GetPlayerController()
{
	return PlayerController;
}

