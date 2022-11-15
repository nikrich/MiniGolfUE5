// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBox.h"
#include "Components/BoxComponent.h"
#include "BasePawn.h"

// Sets default values
ABaseBox::ABaseBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Base Collider"));
	RootComponent = BaseCollider;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box Mesh"));
	BoxMesh->SetupAttachment(BaseCollider);
}

// Called when the game starts or when spawned
void ABaseBox::BeginPlay()
{
	Super::BeginPlay();
	BaseCollider->OnComponentBeginOverlap.AddDynamic(this, &ABaseBox::OnGolfBallEnter);
}

// Called every frame
void ABaseBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseBox::OnGolfBallEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (!OtherComponent->ComponentHasTag("Ball"))
		return;

	auto Material = BoxMesh->GetMaterial(0);	
	BoxMesh->GetOwner()->Destroy();

	if (!Material)
		return;

	OtherComponent->SetMaterial(0, Material);

	if (ShotCameraShake)
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(ShotCameraShake);

	ABasePawn* GolfBall = Cast<ABasePawn>(OtherActor);

	if (!GolfBall)
		return;

	GolfBall->AddColor(Color);
}
