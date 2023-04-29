// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PhantomPawn.h"
#include "Inky.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API AInky : public APhantomPawn
{
	GENERATED_BODY()
		virtual AGridBaseNode* GetPlayerRelativeTarget() override;

public:
	AInky();
};
