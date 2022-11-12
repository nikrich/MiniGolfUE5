// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MINIGOLF_API DirectionHelper
{
public:
	DirectionHelper();
	~DirectionHelper();
	static float GetQuadrantAngle(FVector DirectionalVector);
};
