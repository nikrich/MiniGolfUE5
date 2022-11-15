// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GolfHole.generated.h"

UCLASS()
class MINIGOLF_API AGolfHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGolfHole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* BaseCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
		TArray<FString> BallWinColors;

	class AGolfGameMode* GolfGameMode;	
	class AGolfPlayerController* PlayerController;
	bool IsActive = true;

	void EndRound(class ABasePawn* GolfBall);
	bool DidWin(class ABasePawn* GolfBall);

	UFUNCTION()
		void OnGolfBallEnter(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
