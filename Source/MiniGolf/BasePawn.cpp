// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "GolfHole.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GolfPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Camera/CameraComponent.h"

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

void ABasePawn::Power(float fValue)
{
	PowerValue = fValue;
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CurrentForce = GetForwardForce();

	StopTurnIfBallStops();
	UpdateArrow();
}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &ABasePawn::Shoot);
	PlayerInputComponent->BindAxis(TEXT("Power"), this, &ABasePawn::Power);
}

FVector ABasePawn::GetForwardVector() const
{
	if (!PlayerController)
		return GetOwner()->GetActorForwardVector();

	if (PlayerController->GetIsController()) {
		// TODO: Use DI ISO this boknai
		FMinimalViewInfo CameraViewInfo;
		UCameraComponent* CameraComponent = Cast<UCameraComponent>(GetComponentByClass(UCameraComponent::StaticClass()));
		CameraComponent->GetCameraView(1.0f, CameraViewInfo);

		if (!CameraComponent)
			return FVector(0.f, 0.f, 0.f);

		//return CameraViewInfo.Rotation.GetVe;
		//UE_LOG(LogTemp, Warning, TEXT("Camera Rotation: %f"), CameraViewInfo.Rotation.Yaw);

		auto ForwardVector = CameraViewInfo.Rotation.Vector();
		ForwardVector.Z = 0; // Do not allow ball to go into the air

		return ForwardVector;
	}

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
	if (!PlayerController)
		return 0.f;

	if (PlayerController->GetIsController()) {
		//UE_LOG(LogTemp, Warning, TEXT("Power Value: %f"), PowerValue);		;
		return CurrentForce + PowerValue * 10;
	}	

	//UE_LOG(LogTemp, Warning, TEXT("Power Value: %f"), FVector::Dist(GetActorLocation(), GetMouseCollision()));

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

	if (GetDistance() == 0) {
		ArrowLengthSpringArm->SetVisibility(false, true);
		return;
	}

	ArrowLengthSpringArm->SetVisibility(false);
	FRotator LookAtRotation = FRotator(0.f, 180 + GetForwardVector().Rotation().Yaw, 0.f);
	ArrowLengthSpringArm->SetWorldRotation(FMath::RInterpTo(ArrowLengthSpringArm->GetComponentRotation(), LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 20.f));
	ArrowLengthSpringArm->TargetArmLength = GetForwardForce();

	FVector BodyScale = ArrowBodyMesh->GetRelativeScale3D();
	BodyScale.X = GetForwardForce() / 105;
	ArrowBodyMesh->SetRelativeScale3D(BodyScale);
	ArrowMaterial->SetScalarParameterValue(TEXT("Strength"), GetForwardForce() / MaxForce);
}

AGolfPlayerController* ABasePawn::GetGolfPlayerController() const
{
	return PlayerController;
}

void ABasePawn::AddColor(FString Color)
{
	Colors.Add(Color);

	for (int i = 0; i < Colors.Num(); i++) {
		UE_LOG(LogTemp, Warning, TEXT("Color: %s"), *Colors[i]);
	}

	UpdateBallMaterialByColor();
}

TArray<FString> ABasePawn::GetColors()
{
	return Colors;
}

void ABasePawn::UpdateBallMaterialByColor()
{
	if (Materials.Num() < 7)
		return;

	if (Colors.Contains("blue") && Colors.Contains("red") && Colors.Contains("yellow")) {
		BaseMesh->SetMaterial(0, Materials[6]);
		return;
	}

	if (Colors.Contains("red") && Colors.Contains("yellow")) {
		BaseMesh->SetMaterial(0, Materials[5]);
		return;
	}

	if (Colors.Contains("blue") && Colors.Contains("yellow")) {
		BaseMesh->SetMaterial(0, Materials[4]);
		return;
	}

	if (Colors.Contains("blue") && Colors.Contains("red")) {
		BaseMesh->SetMaterial(0, Materials[3]);
		return;
	}

	if (Colors.Contains("red")) {
		BaseMesh->SetMaterial(0, Materials[2]);
		return;
	}

	if (Colors.Contains("yellow")) {
		BaseMesh->SetMaterial(0, Materials[1]);
		return;
	}

	if (Colors.Contains("blue")) {
		BaseMesh->SetMaterial(0, Materials[0]);
		return;
	}
}

