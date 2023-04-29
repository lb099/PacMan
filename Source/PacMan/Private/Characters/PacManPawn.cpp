// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/PacManPawn.h"

APacManPawn::APacManPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	LastInputDirection = FVector(0, 0, 0);
	LastValidInputDirection = FVector(0, 0, 0);
	CurrentGridCoords = FVector2D(16, 2);

	GameInstance = Cast<UPacManGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
}

void APacManPawn::Tick(float DeltaTime)
{
		//if lives > 0 PacMan can move
		if ((GameInstance->Check_Lives()) > 0) {

			Super::Tick(DeltaTime);
		}
}

void APacManPawn::SetVerticalInput(float AxisValue)
{
		if (AxisValue == 0) return;
		const FVector Dir = (GetActorForwardVector() * AxisValue).GetSafeNormal();
		LastInputDirection = Dir.GetSafeNormal();
		SetNextNodeByDir(LastInputDirection);
}

void APacManPawn::SetHorizontalInput(float AxisValue)
{
		if (AxisValue == 0) return;
		const FVector Dir = (GetActorRightVector() * AxisValue).GetSafeNormal();
		LastInputDirection = Dir;
		SetNextNodeByDir(LastInputDirection);
}

void APacManPawn::OnClick()
{
	FHitResult Hit = FHitResult(ForceInit);
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	if (AGridBaseNode* CurrTile = Cast<AGridBaseNode>(Hit.GetActor()))
	{
		FVector2D CurrCoords = CurrTile->GetGridPosition();

		//display coordinates on click on the map, for debug mode
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Position x=%f  Y=%f "), CurrCoords.X, CurrCoords.Y));
	}
}

void APacManPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Bind Actions here
	InputComponent->BindAxis(TEXT("MoveForward"), this, &APacManPawn::SetVerticalInput);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &APacManPawn::SetHorizontalInput);
	// bind the OnClick function to InputComponent for test purpose
	InputComponent->BindAction("Click", IE_Pressed, this, &APacManPawn::OnClick);
}

void APacManPawn::Reset_LastDirection()
{
	LastInputDirection = FVector(0, 0, 0);
	LastValidInputDirection = FVector(0, 0, 0);
}

void APacManPawn::BeginPlay()
{
	Super::BeginPlay();
	//// posizione iniziale del pawn
	FVector2D StartNode = TheGridGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = TheGridGen->TileMap[StartNode];
}

void APacManPawn::HandleMovement()
{
	Super::HandleMovement();
	// si muove autonomamente fino a che il next node è walkable

	if (!TargetNode && !NextNode) {
		if (TheGridGen->IsNodeValidForWalk(TheGridGen->GetNextNode(CurrentGridCoords, LastInputDirection))) {
			SetLastValidDirection(LastInputDirection);
		}
		//execute if next node is Teleport node
		if (TheGridGen->IsNodeValidTeleport(TheGridGen->GetNextNode(CurrentGridCoords, LastInputDirection))) {

			FVector vLeft(0.f, -1.f, 0.f);
			FVector vRight(0.f, 1.f, 0.f);

			if (LastInputDirection == vLeft) {
				FVector2D v2TelCoords(16.f, 27.f); 
				const auto L_Teleport_Node = TheGridGen->GetNextNode(v2TelCoords, LastValidInputDirection);
				Set_Teleport(true);
				NextNode = L_Teleport_Node;
			}

			else if (LastInputDirection == vRight) {
				FVector2D v2TelCoords(16.f, 0.f);
				const auto R_Teleport_Node = TheGridGen->GetNextNode(v2TelCoords, LastValidInputDirection);
				Set_Teleport(true);
				NextNode = R_Teleport_Node;
			}
		}

		SetNodeGeneric(LastValidInputDirection);
	}
}

void APacManPawn::SetTargetNode(AGridBaseNode* Node)
{
	Super::SetTargetNode(Node);
}

void APacManPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector vResetLastDir(0.f, 0.f, 0.f);
	//overlapped phantom
	const auto PN = Cast<APhantomPawn>(OtherActor);
	if (PN)
	{
		//check if frightened mode isn't active
		if (!(GameInstance->Check_FR())) {
			//if lives > 0 and PacMan not in dead status
			if ((GameInstance->Check_Lives() > 0) && !Check_Dead()) {

				LastInputDirection = vResetLastDir;
				GameInstance->Decrease_Lives();

				FVector vNodeDir(0.f, -1.f, 0.f);
				FVector2D v2NodeDeadCoords(16.f, 5.f);
				const auto NodeDead = TheGridGen->GetNextNode(v2NodeDeadCoords, vNodeDir);
				NextNode = NodeDead;
				Set_Dead(true);
				GameInstance->Set_Message("");
			}

			//if lives = 0 display game over message
			else if (GameInstance->Check_Lives() == 0 && GameInstance->Check_EatenFood() != 244 ) {
				GameInstance->Set_Message("Game Over!");
			}
		}

		//if frightened mode is active, if PacMan overlaps a Phantom, set Phathom to go in the Phantom home
		if (GameInstance->Check_FR()) {
			GameInstance->Set_PhantomGoHome(true, PN);
		}
	}

	//overlapped food
	const auto FoodNode = Cast<AGridBaseNode>(OtherActor);
	//normal food
	if (FoodNode && FoodNode->CanBeEat() && !FoodNode->Check_PowerNode())
	{
		FoodNode->Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FoodNode->StaticMesh->SetVisibility(false);
		FoodNode->bIsEatable = false;

		GameInstance->Increase_Score(10);
		GameInstance->Set_EatenFood(1);
	}
	//Power food. On overlap, frightened mode starts
	else if (FoodNode && FoodNode->Check_PowerNode())
	{
		FoodNode->Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FoodNode->StaticMesh->SetVisibility(false);
		FoodNode->bPowerNode = false;
		
		GameInstance->Increase_Score(50);
		//Set Frightened mode true
		GameInstance->Set_FR(true);
		GameInstance->Set_EatenFood(1);
	}
}
