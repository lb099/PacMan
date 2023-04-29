// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GridGenerator.h"
#include "Grid/PacManGameInstance.h"
#include "Kismet/GameplayStatics.h"

constexpr int MapSizeX = 31;
// la y contiene 28 elementi (+ il carattere di fine stringa)
constexpr int MapSizeY = 29;

// # - Wall
// A - Before Teleport Node
// B - Big Food  
// T - Teleport Node
// R - Ghost Respawn
// N - Nodes without food (walkable)
// I - Invisible Not Walkable
// L - Phantom Home Entrance Node
// Z - Phantom Home Exit node
// F - Phantom Home Node Not Walkable by Phantom

static char Map[MapSizeX][MapSizeY] = {
	"############################",
	"#                          #",
	"# ########## ## ########## #",
	"# ########## ## ########## #",
	"#      ##    ##    ##      #",
	"### ## ## ######## ## ## ###",
	"### ## ## ######## ## ## ###",
	"#B  ##                ##  B#",
	"# #### ##### ## ##### #### #",
	"# #### ##### ## ##### #### #",
	"#            ##            #",
	"###### ##N########N## ######",
	"###### ##N########N## ######",
	"###### ##NNNNNNNNNN## ######",
	"###### ##N########N## ######",
	"###### ##N#FIIIIF#N## ######",
	"TAAAAA NNN#FIFFIF#NNN AAAAAT",
	"###### ##N#FIFFIF#N## ######",
	"###### ##N##ZFFL##N## ######",
	"###### ##NNNNNNNNNN## ######",
	"###### #####N##N##### ######",
	"###### #####N##N##### ######",
	"#      ##   N##N   ##      #",
	"# #### ## ######## ## #### #",
	"# #### ## ######## ## #### #",
	"#                          #",
	"# #### ##### ## ##### #### #",
	"#B#### ##### ## ##### ####B#",
	"# #### ##### ## ##### #### #",
	"#            ##            #",
	"############################",
};

// Sets default values
AGridGenerator::AGridGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TileSize = 100.0f;
	SpawnOffset = FVector(TileSize);

}

// Called when the game starts or when spawned
void AGridGenerator::BeginPlay()
{
	Super::BeginPlay();
	GenerateGrid();
}

// Called every frame
void AGridGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TMap<FVector2D, AGridBaseNode*> AGridGenerator::GetTileMAp()
{
	return TileMap;
}

void AGridGenerator::GenerateGrid()
{
	for (int x = 0; x < MapSizeX; x++)
	{
		for (int y = 0; y < MapSizeY - 1; y++)
		{
			const char MapTile = Map[x][y];

			FVector OffsetVector(x * SpawnOffset.X, y * SpawnOffset.Y, 0);
			// imposto le coordinate spaziali per la funzione di spawn della tile
			const FVector CurrentSpawnPosition = GetActorLocation() + OffsetVector;
			// questa funzione spawna una nuova tile
			const auto SpawnedNode = SpawnNodeActorById(MapTile, CurrentSpawnPosition);
			// assegna le coordinate di griglia alla tile
			SpawnedNode->TileGridPosition = (FVector2D(x, y));
			// assegna le coordinate spaziali alla tile
			SpawnedNode->TileCoordinatesPosition = CurrentSpawnPosition;
			// aggiungo alle strutture dati il riferimento alla tile creata
			Grid.Add(SpawnedNode);
			TileMap.Add(FVector2D(x, y), SpawnedNode);
		}
	}
}

AGridBaseNode* AGridGenerator::GetNodeByCoords(const FVector2D Coords)
{
	if (Coords.X > MapSizeX - 1 || Coords.Y > MapSizeY - 2) return nullptr;
	if (Coords.X < 0 || Coords.Y < 0) return nullptr;
	return GetTileMAp()[FVector2D(Coords.X, Coords.Y)];
}

bool AGridGenerator::IsNodeReachableAndNextToCurrentPosition(const FVector2D CurrentCoordinates, const FVector2D TargetCoords)
{
	const float DistX = FMath::Abs(CurrentCoordinates.X - TargetCoords.X);
	const float DistY = FMath::Abs(CurrentCoordinates.Y - TargetCoords.Y);
	if (DistX > 1 || DistY > 1) return false;
	AGridBaseNode* const N = GetNodeByCoords(TargetCoords);
	if (N && N->EWalkableId == NotWalkable) return false;
	return true;
}

AGridBaseNode* AGridGenerator::GetClosestNodeFromMyCoordsToTargetCoords(const FVector2D StartCoords, const FVector2D TargetCoords, AGridPawn* PN, FVector IgnoredDir)
{
	UPacManGameInstance* GameInstance;

	GameInstance = Cast<UPacManGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	//Get all neighbours
	const TArray<FDirNode> Neighbours = GetNodeNeighbours(GetNodeByCoords(StartCoords));
	float Dist = FLT_MAX;
	AGridBaseNode* ReturnNode = nullptr;
	for (FDirNode FDirNode : Neighbours)
	{
		if (FDirNode.Node == nullptr || FDirNode.Dir == IgnoredDir || FDirNode.Node->EWalkableId == NotWalkable || FDirNode.Node->EPreTelId == PreTel)
			continue;
		
		
		if (FDirNode.Node->EWalkableId == EnterHome && !GameInstance->Check_FR() || (FDirNode.Node->EWalkableId == ExitHome && !GameInstance->Check_PhantomExitHome(PN))) {
			continue;
		}

		const float TempDist = FVector2D::Distance(FDirNode.Node->GetGridPosition(), TargetCoords);
		if (TempDist < Dist)
		{
			Dist = TempDist;
			ReturnNode = FDirNode.Node;
		}
	}

	return ReturnNode;
}

TArray<FDirNode> AGridGenerator::GetNodeNeighbours(const AGridBaseNode* Node)
{
	TArray<FDirNode> Vec;
	if (Node)
	{
		Vec.Add(FDirNode(GetNextNode(Node->GetGridPosition(), FVector::ForwardVector), FVector::ForwardVector));
		Vec.Add(FDirNode(GetNextNode(Node->GetGridPosition(), FVector::RightVector), FVector::RightVector));
		Vec.Add(FDirNode(GetNextNode(Node->GetGridPosition(), -FVector::ForwardVector), -FVector::ForwardVector));
		Vec.Add(FDirNode(GetNextNode(Node->GetGridPosition(), FVector::LeftVector), FVector::LeftVector));
	}
	return Vec;
}


AGridBaseNode* AGridGenerator::SpawnNodeActorById(char CharId, FVector Position) const
{
	AGridBaseNode* Node;
	TSubclassOf<AGridBaseNode> ClassToSpawn = AGridBaseNode::StaticClass();

	if (CharId == '#')
	{
		ClassToSpawn = WallNode;
	}
	else if (CharId == 'B')
	{
		ClassToSpawn = PowerNode;
	}
	else if (CharId == 'T')
	{
		ClassToSpawn = TeleportNode;
	}
	else if (CharId == 'N')
	{
		ClassToSpawn = GhostAreaNode;
	}
	else if (CharId == 'I')
	{
		ClassToSpawn = GhostHomeNode;
	}
	else if (CharId == 'Z')
	{
		ClassToSpawn = GhostHomeExitNode;
	}
	else if (CharId == 'L')
	{
		ClassToSpawn = GhostHomeEnterNode;
	}
	else if (CharId == 'F')
	{
		ClassToSpawn = GhostHomeForbiddenNode;
	}
	else if (CharId == 'A') {
		ClassToSpawn = BeforeTeleportNode;
	}
	else
	{
		//spawn tile and food
		ClassToSpawn = LabyrinthNode;
		ClassToSpawn = PointNode;
	}
	Node = GetWorld()->SpawnActor<AGridBaseNode>(ClassToSpawn, Position, FRotator::ZeroRotator);
	return Node;
}


bool AGridGenerator::IsNodeValidForWalk(AGridBaseNode* Node)
{
	if (Node == nullptr) return false;
	if (Node->EWalkableId == NotWalkable) return false;
	if (Node->EWalkableId == EnterHome) return false;
	if (Node->EWalkableId == ExitHome) return false;

	return true;
}
//check if node is a Teleport
bool AGridGenerator::IsNodeValidTeleport(AGridBaseNode* Node)
{
	if (Node == nullptr) return false;
	if (Node->EWalkableId == Teleport) return true;

	return false;
}
//check if node is in Phantom Home
bool AGridGenerator::IsNodeValidHome(AGridBaseNode* Node)
{
	if (Node == nullptr) return false;
	if (Node->EWalkableId == Home) return true;

	return false;
}
//check if node is the entrance of phantom home
bool AGridGenerator::IsNodeValidEnterHome(AGridBaseNode* Node)
{
	if (Node == nullptr) return false;
	if (Node->EWalkableId == EnterHome) return true;

	return false;
}
//check if the node is the exit node from phantom home
bool AGridGenerator::IsNodeValidExitHome(AGridBaseNode* Node)
{
	if (Node == nullptr) return false;
	if (Node->EWalkableId == ExitHome) return true;

	return false;
}



AGridBaseNode* AGridGenerator::GetNextNode(const FVector2D StartCoords, FVector InputDir)
{

	const float RequestedX = StartCoords.X + InputDir.X;
	const float RequestedY = StartCoords.Y + InputDir.Y;
	// la funzione clamp ritorna un numero compreso tra min e max se nel range
	//se più piccolo del min ritorna il min
	//se più grande del max ritorna il max
	const float ClampedX = FMath::Clamp(RequestedX, 0.f, MapSizeX - 1);
	const float ClampedY = FMath::Clamp(RequestedY, 0.f, MapSizeY - 2);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("x=%f,y=%f"), ClampedX, ClampedY));
	AGridBaseNode* PossibleNode = GetTileMAp()[FVector2D(ClampedX, ClampedY)];

	if (RequestedX > ClampedX || RequestedX < 0)
	{
		//X overflow
		PossibleNode = nullptr;
	}
	if (RequestedY > ClampedY || RequestedY < 0)
	{
		//Y overflow
		PossibleNode = nullptr;
	}
	return PossibleNode;
}

FVector2D AGridGenerator::GetPosition(const FHitResult& Hit)
{
	return Cast<AGridBaseNode>(Hit.GetActor())->GetGridPosition();
}

TArray<AGridBaseNode*>& AGridGenerator::GetTileArray()
{
	return Grid;
}

FVector AGridGenerator::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY)
{
	return  TileSize * FVector(InX, InY, 0);
}

FVector2D AGridGenerator::GetXYPositionByRelativeLocation(const FVector& Location)
{
	double x = floor(Location[0] / (TileSize));
	double y = floor(Location[1] / (TileSize));
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("x=%f,y=%f"), x, y));
	return FVector2D(x, y);
}

FVector2D AGridGenerator::GetTwoDOfVector(FVector DDDVector)
{
	return FVector2D(DDDVector.X, DDDVector.Y);
}

FVector AGridGenerator::GetThreeDOfTwoDVector(FVector2D DDDVector)
{
	return FVector(DDDVector.X, DDDVector.Y, 0);
}

