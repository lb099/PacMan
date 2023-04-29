// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GridPawn.h"
#include "Grid/GridPlayerController.h"
#include "Grid/GridGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGridPawn::AGridPawn()
{
	//
	// N.B il pawn viene spawnato automaticamente nella posizione del player start
	// dato che il pawn di default è stato impostato nei setting come BP_GridPawn
	//  
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(Collider);

	//// nodi
	LastNode = nullptr;
	TargetNode = nullptr;
	NextNode = nullptr;

	GameInstance = Cast<UPacManGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

}

void AGridPawn::SetVerticalInput(float AxisValue)
{
}

void AGridPawn::SetHorizontalInput(float AxisValue)
{
}

// Called when the game starts or when spawned
void AGridPawn::BeginPlay()
{
	Super::BeginPlay();
	GameMode = (AGridGameMode*)(GetWorld()->GetAuthGameMode());
	TheGridGen = GameMode->GField;
	CurrentMovementSpeed = NormalMovementSpeed;
	bCanMove = true;
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AGridPawn::OnOverlapBegin);

}

void AGridPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

// Called to bind functionality to input
void AGridPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Called every frame
void AGridPawn::Tick(float DeltaTime)
{
	//check if can exec
	if (GameInstance->Check_Exec()) {

		Super::Tick(DeltaTime);
		HandleMovement();

	}

}

void AGridPawn::OnClick()
{
}

void AGridPawn::HandleMovement()
{
	MoveToCurrentTargetNode();
	if (TargetNode == nullptr)
	{
		if (NextNode != nullptr)
		{
			SetTargetNode(NextNode);
			SetNextNode(nullptr);
		}
	}
}

void AGridPawn::MoveToCurrentTargetNode()
{
	//read first the dead state
	bool bCheck_dead = false;
	FVector2D v2Dest;


	if (Check_Dead()) {
		bCheck_dead = true;
		FVector vDir(0.f, -1.f, 0.f);
		FVector2D v2Dead_Node_Coords(16.f, 5.f);
		const auto DeadNode = TheGridGen->GetNextNode(v2Dead_Node_Coords, vDir);
		TargetNode = DeadNode;
	}

	if (TargetNode == nullptr) return;
	const float Dist = FMath::Abs(FVector::Dist2D(TargetNode->GetActorLocation(), GetActorLocation()));
	if (Dist <= AcceptedDistance)
	{
		OnNodeReached();
		return;
	}
	// funzione di interpolazione che fa muovere il pawn verso una nuova posizione data la posizione corrente
	const FVector2D StartVector = TheGridGen->GetTwoDOfVector(GetActorLocation());
	const FVector2D EndVector = TheGridGen->GetTwoDOfVector(TargetNode->GetActorLocation());

	//if teleport change speed to TeleportMovementSpeed
	if (Check_Teleport()) {
		const auto Pos = FMath::Vector2DInterpConstantTo(StartVector, EndVector, GetWorld()->GetDeltaSeconds(), TeleportMovementSpeed);
		v2Dest = Pos;
	}
	//if dead change speed to DeadMovementSpeed
	else if (bCheck_dead) {
		const FVector2D EndVectorDead = TheGridGen->GetTwoDOfVector(TargetNode->GetActorLocation());
		const auto Pos = FMath::Vector2DInterpConstantTo(StartVector, EndVectorDead, GetWorld()->GetDeltaSeconds(), DeadMovementSpeed);
		v2Dest = Pos;
		OnNodeReached();

	}
	//else use CurrentMovementSpeed
	else {
		const auto Pos = FMath::Vector2DInterpConstantTo(StartVector, EndVector, GetWorld()->GetDeltaSeconds(), CurrentMovementSpeed);
		v2Dest = Pos;
	}
	//check if this is a Phantom
	if (GameInstance->Check_EPhantomName(this) != "") {
		
		//if the this Phantom has to go to Phantom Home and Frightened mode is active
		if (GameInstance->Check_PhantomGoHome(this) && GameInstance->Check_FR()) {

			const auto Pos = FMath::Vector2DInterpConstantTo(StartVector, EndVector, GetWorld()->GetDeltaSeconds(), HomeMovementSpeed);
			v2Dest = Pos;

		}
		//else use Frightened mode speed
		else if ((GameInstance->Check_FR())) {

			const auto Pos = FMath::Vector2DInterpConstantTo(StartVector, EndVector, GetWorld()->GetDeltaSeconds(), FrightenedMovementSpeed);
			v2Dest = Pos;
		}

	}
		
	const FVector Location(v2Dest.X, v2Dest.Y, GetActorLocation().Z);
	SetActorLocation(Location);

	//reset to false Teleport and Dead status if active 
	if (Check_Teleport()) {

		Set_Teleport(false);
	}
	else if (bCheck_dead) {
		Set_Dead(false);
		Set_CanMove(false);
	}
}

void AGridPawn::SetNodeGeneric(const FVector Dir)
{

	if ((Check_Teleport()) || (!Check_CanMove())) {
		//do nothing
	}

	else {

		const auto Node = TheGridGen->GetNextNode(CurrentGridCoords, Dir);
		if (TheGridGen->IsNodeValidForWalk(Node)) {
			SetTargetNode(Node);
		}

		if ((TheGridGen->IsNodeValidEnterHome(Node)) && (GameInstance->Check_EPhantomName(this) != "")) {
			SetTargetNode(Node);
		}

		else if ((TheGridGen->IsNodeValidHome(Node)) && (GameInstance->Check_EPhantomName(this) != "")) {
			GameInstance->Set_PhantomExitHome(true, this);
			SetTargetNode(Node);

		}

		else if ((TheGridGen->IsNodeValidExitHome(Node)) && (GameInstance->Check_EPhantomName(this) != "")) {
			GameInstance->Set_PhantomExitHome(false, this);
			SetTargetNode(Node);
		}

	}
}

void AGridPawn::SetNextNodeByDir(FVector InputDir, bool ForceLast)
{
	const FVector2D Coords = TargetNode ? TargetNode->GetGridPosition() : LastNode->GetGridPosition();
	const auto Node = GameMode->GField->GetNextNode(Coords, InputDir);

	if (Check_Dead()) {
		//do nothing
	}

	else {

		if (GameMode->GField->IsNodeValidForWalk(Node))
		{
			SetNextNode(Node);
			SetLastValidDirection(InputDir);
		}

		if ((TheGridGen->IsNodeValidEnterHome(Node)) && (GameInstance->Check_EPhantomName(this) != "")) {
			
			SetNextNode(Node);
			SetLastValidDirection(InputDir);
		}

		if ((TheGridGen->IsNodeValidHome(Node)) && (GameInstance->Check_EPhantomName(this) != "")) {
			GameInstance->Set_PhantomExitHome(true, this);
			SetNextNode(Node);
			SetLastValidDirection(InputDir);

		}

		if ((TheGridGen->IsNodeValidExitHome(Node)) && (GameInstance->Check_EPhantomName(this) != "")) {
			GameInstance->Set_PhantomExitHome(false, this);
			SetNextNode(Node);
			SetLastValidDirection(InputDir);
		}

		if (GameMode->GField->IsNodeValidTeleport(Node)) {

			FVector vLeft(0.f, -1.f, 0.f);
			FVector vRight(0.f, 1.f, 0.f);

			if (LastInputDirection == vLeft) {
				FVector2D v2TelCoordsLeft(16.f, 27.f);
				const auto TeleportNode = GameMode->GField->GetNextNode(v2TelCoordsLeft, LastValidInputDirection);
				Set_Teleport(true);
				NextNode = TeleportNode;
				SetLastValidDirection(InputDir);
			}

			else if (LastInputDirection == vRight) {
				FVector2D v2TelCoordsRight(16.f, 0.f);
				const auto TeleportNode = GameMode->GField->GetNextNode(v2TelCoordsRight, LastValidInputDirection);
				Set_Teleport(true);
				NextNode = TeleportNode;
				SetLastValidDirection(InputDir);
			}

		}
		
	}
}

void AGridPawn::SetTargetNode(AGridBaseNode* Node)
{
	TargetNode = Node;
}

void AGridPawn::SetNextNode(AGridBaseNode* Node)
{
	NextNode = Node;
}

void AGridPawn::OnNodeReached()
{
	CurrentGridCoords = TargetNode->GetGridPosition();
	LastNode = TargetNode;
	SetTargetNode(nullptr);
}

FVector AGridPawn::GetLastValidDirection() const
{
	return LastValidInputDirection;
}

AGridBaseNode* AGridPawn::GetLastNode() const
{
	return LastNode;
}

AGridBaseNode* AGridPawn::GetTargetNode() const
{
	return TargetNode;
}

FVector2D AGridPawn::GetLastNodeCoords() const
{
	if (LastNode)
	{
		return LastNode->GetGridPosition();
	}
	return FVector2D(0, 0);
}

FVector2D AGridPawn::GetTargetNodeCoords() const
{
	if (TargetNode)
	{
		return TargetNode->GetGridPosition();
	}
	return FVector2D::ZeroVector;
}

void AGridPawn::SetLastValidDirection(FVector Dir)
{
	if (Dir == FVector::ZeroVector) return;
	LastValidInputDirection = Dir;
}

