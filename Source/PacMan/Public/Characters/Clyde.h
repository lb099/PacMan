// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PhantomPawn.h"
#include "Clyde.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API AClyde : public APhantomPawn
{
	GENERATED_BODY()
		virtual AGridBaseNode* GetPlayerRelativeTarget() override;

public:
	AClyde();
	
};
