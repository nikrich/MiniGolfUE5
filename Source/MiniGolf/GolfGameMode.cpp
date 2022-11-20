// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfGameMode.h"
#include <Kismet/GameplayStatics.h>
#include "GolfPlayerController.h"

void AGolfGameMode::BeginPlay()
{
	Super::BeginPlay();
	HandleGameStart();
}

void AGolfGameMode::HandleGameStart()
{
	GolfPlayerController = Cast<AGolfPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if (GolfPlayerController) {
		GolfPlayerController->SetPlayerEnabledState(true);
	}
}

AGolfPlayerController* AGolfGameMode::GetGolfPlayerController() const
{
	return GolfPlayerController;
}
