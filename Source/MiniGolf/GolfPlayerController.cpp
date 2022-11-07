// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfPlayerController.h"

void AGolfPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
	if (bPlayerEnabled) {
		GetPawn()->EnableInput(this);
	}
	else {
		GetPawn()->DisableInput(this);
	}

	bShowMouseCursor = bPlayerEnabled;
}