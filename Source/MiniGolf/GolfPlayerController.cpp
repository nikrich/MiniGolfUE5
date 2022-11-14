// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfPlayerController.h"

void AGolfPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
	if (!GetPawn()) return;

	if (bPlayerEnabled) {
		GetPawn()->EnableInput(this);
	}
	else {
		GetPawn()->DisableInput(this);
	}

	IsPlayerEnabled = bPlayerEnabled;
}

bool AGolfPlayerController::GetPlayerEnabledState()
{
	return IsPlayerEnabled;
}

void AGolfPlayerController::Shoot()
{	
	ShotsTaken++;
}

int32 AGolfPlayerController::GetShotsTaken()
{
	return ShotsTaken;
}

bool AGolfPlayerController::GetIsController() const
{
	return IsController;
}

void AGolfPlayerController::SetIsController(bool bIsController)
{
	IsController = bIsController;
}
