// Fill out your copyright notice in the Description page of Project Settings.


#include "DirectionHelper.h"

DirectionHelper::DirectionHelper()
{
}

DirectionHelper::~DirectionHelper()
{
}

float DirectionHelper::GetQuadrantAngle(FVector DirectionalVector)
{
	float Direction = FMath::Atan(DirectionalVector.Y / DirectionalVector.X);
	float Degrees = FMath::RadiansToDegrees(Direction);
	//UE_LOG(LogTemp, Warning, TEXT("Forward Vector Direction: %f"), Degrees);
	

	//UE_LOG(LogTemp, Warning, TEXT("Ball - X: %f, Y: %f, Z: %f"), DirectionalVector.X, DirectionalVector.Y, DirectionalVector.Z);

	// Quadrant 1
	if (DirectionalVector.X >= 0 && DirectionalVector.Y >= 0)
		return Degrees;

	// Quadrant 2
	if (DirectionalVector.X < 0 && DirectionalVector.Y >= 0)
		return 180.f + Degrees;

	// Quadrant 3
	if (DirectionalVector.X < 0 && DirectionalVector.Y < 0)
		return 180.f + Degrees;

	// Quadrant 4
	if (DirectionalVector.X >= 0 && DirectionalVector.Y < 0)
		return 360.f + Degrees;

	return 0.0f;
}
