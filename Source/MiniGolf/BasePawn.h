// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class MINIGOLF_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
		
	void Shoot();
	void Power(float fValue);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* ArrowLengthSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ArrowHeadMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ArrowBodyMesh;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float SpeedMultiplier = 200.f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float MaxForce = 1300.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		TSubclassOf<class UCameraShakeBase> ShotCameraShake;

	UPROPERTY(EditAnywhere, Category = "Movement")
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Attributes")
		TArray<UMaterial*> Materials;
	
	AActor* GolfHole;
	class AGolfPlayerController* PlayerController;
	class UMaterialInstanceDynamic* ArrowMaterial;
	bool CanShoot = true;
	float PowerValue = 0.f;
	FVector GetForwardVector() const;
	void StopTurnIfBallStops();
	float GetDistance() const;
	FVector GetMouseCollision() const;
	void UpdateArrow();

	// Box Attributes
	TArray<FString> Colors = TArray<FString>();

	UFUNCTION(Blueprintcallable)
	float GetForwardForce() const;
	void UpdateBallMaterialByColor();

public:
	UFUNCTION(Blueprintcallable)
		AGolfPlayerController* GetGolfPlayerController() const;

	void AddColor(FString Color);
	TArray<FString> GetColors();
};
