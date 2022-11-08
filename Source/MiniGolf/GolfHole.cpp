// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfHole.h"
#include "Components/BoxComponent.h"
#include <Kismet/GameplayStatics.h>
#include "BasePawn.h"
#include "GolfGameMode.h"
#include "GolfPlayerController.h"

// Sets default values
AGolfHole::AGolfHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Base Collider"));
	RootComponent = BaseCollider;

}

// Called when the game starts or when spawned
void AGolfHole::BeginPlay()
{
	Super::BeginPlay();

	BaseCollider->OnComponentHit.AddDynamic(this, &AGolfHole::OnGolfBallEnter);
	GolfGameMode = Cast<AGolfGameMode>(UGameplayStatics::GetGameMode(this));
	PlayerController = Cast<AGolfPlayerController>(GetWorld()->GetFirstPlayerController());
}

// Called every frame
void AGolfHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGolfHole::EndRound(ABasePawn* GolfBall)
{
	if (!PlayerController) return;

	PlayerController->SetPlayerEnabledState(false);

	if (!GolfGameMode) return;

	GolfGameMode->RoundComplete(PlayerController->GetShotsTaken());
	IsActive = false;

	if (!PlayerController) return;
}

void AGolfHole::OnGolfBallEnter(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!IsActive) return;

	ABasePawn* GolfBall = Cast<ABasePawn>(OtherActor);

	if (!GolfBall) return;

	EndRound(GolfBall);
}

