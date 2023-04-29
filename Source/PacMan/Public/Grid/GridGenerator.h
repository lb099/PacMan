// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid/GridBaseNode.h"
#include "GridGenerator.generated.h"

struct FDirNode
{
	AGridBaseNode* Node;
	FVector Dir;

	FDirNode(AGridBaseNode* Node, const FVector& Dir) : Node(Node), Dir(Dir)
	{
	}
};

UCLASS()
class PACMAN_API AGridGenerator : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<AGridBaseNode> WallNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<AGridBaseNode> PowerNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<AGridBaseNode> PointNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<AGridBaseNode> LabyrinthNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<AGridBaseNode> TeleportNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<AGridBaseNode> BeforeTeleportNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<AGridBaseNode> GhostAreaNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<AGridBaseNode> GhostHomeNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<AGridBaseNode> GhostHomeExitNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<AGridBaseNode> GhostHomeEnterNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<AGridBaseNode> GhostHomeForbiddenNode;
	
public:	
	// Sets default values for this actor's properties
	AGridGenerator();

	UPROPERTY(VisibleAnywhere)
		class AGridPawn* Phantom;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//given a position returns a tile
	TMap<FVector2D, AGridBaseNode*> TileMap;

	TMap<FVector2D, AGridBaseNode*> GetTileMAp();

	// tile size
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float TileSize;

	UPROPERTY(VisibleAnywhere)
		TArray<AGridBaseNode*> Grid;

	// return a (x,y) position given a hit (click) on a field tile
	FVector2D GetPosition(const FHitResult& Hit);

	// return the array of tile pointers
	TArray<AGridBaseNode*>& GetTileArray();

	// return a relative position given (x,y) position
	FVector GetRelativeLocationByXYPosition(const int32 InX, const int32 InY);

	// return (x,y) position given a relative position
	FVector2D GetXYPositionByRelativeLocation(const FVector& Location);

	FVector2D GetTwoDOfVector(FVector DDDVector);
	FVector GetThreeDOfTwoDVector(FVector2D DDDVector);

	AGridBaseNode* GetNodeByCoords(const FVector2D Coords);
	bool IsNodeReachableAndNextToCurrentPosition(const FVector2D CurrentCoordinates, const FVector2D TargetCoords);
	AGridBaseNode* GetClosestNodeFromMyCoordsToTargetCoords(const FVector2D StartCoords, const FVector2D TargetCoords, AGridPawn* PN, FVector IgnoredDir = FVector::ZeroVector);
	TArray< FDirNode> GetNodeNeighbours(const AGridBaseNode* Node);

	static bool IsNodeValidForWalk(class AGridBaseNode* Node);
	static bool IsNodeValidTeleport(class AGridBaseNode* Node);
	static bool IsNodeValidHome(class AGridBaseNode* Node);
	static bool IsNodeValidEnterHome(class AGridBaseNode* Node);
	static bool IsNodeValidExitHome(class AGridBaseNode* Node);

	AGridBaseNode* GetNextNode(const FVector2D StartCoords, FVector InputDir);

private:

	AGridBaseNode* SpawnNodeActorById(char CharId, FVector Position) const;

	UPROPERTY(EditDefaultsOnly)
		FVector SpawnOffset;

	void GenerateGrid();
};
