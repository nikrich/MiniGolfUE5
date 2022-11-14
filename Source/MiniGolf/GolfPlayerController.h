// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GolfPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MINIGOLF_API AGolfPlayerController : public APlayerController
{
	GENERATED_BODY()
		
private:
	bool IsPlayerEnabled;
	int32 ShotsTaken = 0;
	
public:
	void SetPlayerEnabledState(bool bPlayerEnabled);
	bool GetPlayerEnabledState();
	void Shoot();

	bool IsController = true;

	UFUNCTION(BlueprintCallable)
		int32 GetShotsTaken();

	UFUNCTION(BlueprintCallable)
		bool GetIsController() const;

	UFUNCTION(BlueprintCallable)
		void SetIsController(bool bIsController);
};
