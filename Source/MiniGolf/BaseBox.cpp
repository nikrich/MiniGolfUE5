// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBox.h"
#include "Components/BoxComponent.h"
#include "BasePawn.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ABaseBox::ABaseBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Base Collider"));
	RootComponent = BaseCollider;
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

void ABaseBox::DestroyActor()
{
	Destroy();
}

void ABaseBox::OnGolfBallEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (BreakSound) {
		UGameplayStatics::PlaySoundAtLocation(this, BreakSound, GetActorLocation());
	}

	if (!OtherComponent->ComponentHasTag("Ball"))
		return;
	
	// TODO - fix delay
	//GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ABaseBox::DestroyActor, 4.f, true);
	DestroyActor();	

	if (ShotCameraShake)
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(ShotCameraShake);

	ABasePawn* GolfBall = Cast<ABasePawn>(OtherActor);

	if (!GolfBall)
		return;

	GolfBall->AddColor(Color);
}