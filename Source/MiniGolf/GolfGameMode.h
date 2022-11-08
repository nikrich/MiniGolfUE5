// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GolfGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MINIGOLF_API AGolfGameMode : public AGameModeBase
{
	GENERATED_BODY()	

protected:
	virtual void BeginPlay();	

private:
	class AGolfPlayerController* GolfPlayerController;
	void HandleGameStart();

public:
	UFUNCTION(BlueprintImplementableEvent)
		void RoundComplete(int32 ShotsTaken);

};
