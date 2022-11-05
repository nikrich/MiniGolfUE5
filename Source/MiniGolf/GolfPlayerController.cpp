// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfPlayerController.h"

void AGolfPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
	UE_LOG(LogTemp, Warning, TEXT("Player enabled %b"), bPlayerEnabled);

	if (bPlayerEnabled) {
		GetPawn()->EnableInput(this);
	}
	else {
		GetPawn()->DisableInput(this);
	}

	bShowMouseCursor = bPlayerEnabled;
}