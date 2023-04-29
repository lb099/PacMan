// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PhantomPawn.h"
#include "Characters/PacManPawn.h"
#include "Kismet/GameplayStatics.h"

APhantomPawn::APhantomPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GameInstance = Cast<UPacManGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void APhantomPawn::BeginPlay()
{
	Super::BeginPlay();
	FVector2D StartNode = TheGridGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = TheGridGen->TileMap[StartNode];
	Player = Cast<APacManPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacManPawn::StaticClass()));
}

void APhantomPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	

}

void APhantomPawn::OnNodeReached()
{
	Super::OnNodeReached();
}

void APhantomPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (this->GetTargetNode() == nullptr)
	{
		SetGhostTarget();
	}
}

APacManPawn* APhantomPawn::GetPlayer() const
{
	return Player;
}

void APhantomPawn::SetSpeed(float Speed)
{
	CurrentMovementSpeed = Speed;
}

AGridBaseNode* APhantomPawn::GetPlayerRelativeTarget()
{
	return Player->GetLastNode();
}

void APhantomPawn::SetGhostTarget()
{
	//Target=PacMan
	const AGridBaseNode* Target = GetPlayerRelativeTarget();
	//In Frightened mode the target is a point in the map
	const AGridBaseNode* TargetFrightened;
	//target is phantom home
	const AGridBaseNode* TargetHome;

	FVector vLeft(0.f, -1.f, 0.f);
	FVector2D v2FRCoords(13.f, 14.f);
	FVector2D HomeCoord(16.f, 13.f);

	if (!Target)
	{
		Target = GetPlayer()->GetLastNode();
	}

	AGridBaseNode* PossibleNode = nullptr;

	TargetFrightened = TheGridGen->GetNextNode(v2FRCoords, vLeft);
	TargetHome = TheGridGen->GetNextNode(HomeCoord, vLeft);

	//if frightened change target
	if (GameInstance->Check_FR()) {
		//if phantom reached by PacMan during Frigtened mode, goes to phantom home
		if (GameInstance->Check_PhantomGoHome(this)) {

			PossibleNode = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), TargetHome->GetGridPosition(), this, -(this->GetLastValidDirection()));

		}
		// go to a point in the map
		else {

			PossibleNode = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), TargetFrightened->GetGridPosition(), this, -(this->GetLastValidDirection()));

		}

	}

	//if not frightened
	else {
		//if this=Pinky, sets PossibleNode for Pinky
		if (GameInstance->Check_EPhantomName(this) == "Pinky")
		{
			PossibleNode = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target->GetGridPosition()+4, this, -(this->GetLastValidDirection()));
		}
		//other phantoms
		else {
			PossibleNode = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target->GetGridPosition(), this, -(this->GetLastValidDirection()));
		}
	}

	if (PossibleNode)
	{
			this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode->GetGridPosition() - this->GetLastNodeCoords()), true);
	}
}
