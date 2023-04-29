// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid/GridGenerator.h"
#include "Grid/GridPawn.h"
#include "Characters/Blinky.h"
#include "Characters/Inky.h"
#include "Characters/Clyde.h"
#include "Characters/Pinky.h"
#include "GameFramework/GameMode.h"
#include "GridGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API AGridGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	// TSubclassOf is a template class that provides UClass type safety.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AGridGenerator> GridGeneratorClass;

	// reference to a GameField object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AGridGenerator* GField;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ABlinky> BlinkyClass;
	
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AInky> InkyClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AClyde> ClydeClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<APinky> PinkyClass;
	
	UPROPERTY(VisibleAnywhere)
		ABlinky* BlinkyPawn;

	UPROPERTY(VisibleAnywhere)
		AInky* InkyPawn;

	UPROPERTY(VisibleAnywhere)
		AClyde* ClydePawn;

	UPROPERTY(VisibleAnywhere)
		APinky* PinkyPawn;

	AGridGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
