// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBox.generated.h"

UCLASS()
class MINIGOLF_API ABaseBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBox();	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* BaseCollider;

	UPROPERTY(EditAnywhere, Category = "Components")
		TSubclassOf<class UCameraShakeBase> ShotCameraShake;

	UPROPERTY(EditAnywhere, Category = "Attributes")
		FString Color;

	UPROPERTY(EditAnywhere, Category = "Movement")
		USoundBase* BreakSound;

	FTimerHandle DestroyTimerHandle;

	void DestroyActor();

	UFUNCTION()
		void OnGolfBallEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

};