// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PhantomPawn.h"
#include "Pinky.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API APinky : public APhantomPawn
{
	GENERATED_BODY()
		virtual AGridBaseNode* GetPlayerRelativeTarget() override;

public:
	APinky();
};
