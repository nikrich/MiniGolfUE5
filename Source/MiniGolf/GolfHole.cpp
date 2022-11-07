// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfHole.h"
#include "Components/BoxComponent.h"

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
	
}

// Called every frame
void AGolfHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGolfHole::OnGolfBallEnter(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("In the hole!"));
}

