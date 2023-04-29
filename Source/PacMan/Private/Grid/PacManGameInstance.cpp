// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/PacManGameInstance.h"
#include "Grid/GridPawn.h"

void UPacManGameInstance::Set_FR(bool pIsFR)
{
	bIsFR = pIsFR;
}

bool UPacManGameInstance::Check_FR()
{
	return bIsFR;
}

void UPacManGameInstance::Set_Exec(bool pExec)
{
	bExec = pExec;
}

bool UPacManGameInstance::Check_Exec()
{
	return bExec;
}

void UPacManGameInstance::Increase_Score(int32 pScore)
{
	xScore = xScore + pScore;
}

void UPacManGameInstance::Set_Score(int pScore)
{
	xScore = pScore;
}

int UPacManGameInstance::Check_Score()
{
	return xScore;
}

void UPacManGameInstance::Decrease_Lives()
{
	xLives = xLives - 1;
}

void UPacManGameInstance::Set_Lives(int32 pLives)
{
	xLives = pLives;
}

int32 UPacManGameInstance::Check_Lives() {
	return xLives;
}

void UPacManGameInstance::Set_PhantomGoHome(bool PNHome, AGridPawn* PN)
{

	if (PN->EPhantomName == Blinky) {

		bPNHomeB = PNHome;

	}

	else if (PN->EPhantomName == Inky) {

		bPNHomeI = PNHome;

	}

	else if (PN->EPhantomName == Pinky) {

		bPNHomeP = PNHome;
	}
	
	else if (PN->EPhantomName == Clyde) {

		bPNHomeC = PNHome;

	}
}

bool UPacManGameInstance::Check_PhantomGoHome(AGridPawn* PN)
{

	if (PN->EPhantomName == Blinky) {

		return bPNHomeB;

	}

	else if (PN->EPhantomName == Clyde) {

		return bPNHomeC;

	}

	else if (PN->EPhantomName == Inky) {

		return bPNHomeI;

	}

	else if (PN->EPhantomName == Pinky) {

		return bPNHomeP;
	}

	else {
		return false;
	}


}

void UPacManGameInstance::Set_AllPhantomHome(bool PNHome)
{
	bPNHomeB = PNHome;
	bPNHomeC = PNHome;
	bPNHomeI = PNHome;
	bPNHomeP = PNHome;
}

void UPacManGameInstance::Set_PhantomExitHome(bool PNCanExitHome, AGridPawn* PN)
{
	if (PN->EPhantomName == Blinky) {
		bPNCanExitHomeB = PNCanExitHome;
	}

	else if (PN->EPhantomName == Clyde) {
		bPNCanExitHomeC = PNCanExitHome;

	}

	else if (PN->EPhantomName == Inky) {

		bPNCanExitHomeI = PNCanExitHome;

	}

	else if (PN->EPhantomName == Pinky) {

		bPNCanExitHomeP = PNCanExitHome;
	}
}

bool UPacManGameInstance::Check_PhantomExitHome(AGridPawn* PN)
{
	if (PN->EPhantomName == Blinky) {
		return bPNCanExitHomeB;

	}

	else if (PN->EPhantomName == Clyde) {
		return bPNCanExitHomeC;

	}

	else if (PN->EPhantomName == Inky) {

		return bPNCanExitHomeI;

	}

	else if (PN->EPhantomName == Pinky) {

		return bPNCanExitHomeP;
	}

	else {
		return false;
	}
}

FString UPacManGameInstance::Check_EPhantomName(AGridPawn* PN)
{
	FString sVuoto = "";
	if (PN->EPhantomName == Pinky)
	{
		FString sName = "Pinky";
		return sName;
	}

	else if (PN->EPhantomName == Blinky)
	{
		FString sName = "Blinky";
		return sName;
	}

	else if (PN->EPhantomName == Inky)
	{
		FString sName = "Inky";
		return sName;
	}

	if (PN->EPhantomName == Clyde)
	{
		FString sName = "Clyde";
		return sName;
	}
	return sVuoto;
}

void UPacManGameInstance::Set_Message(FString pMessage)
{
	sMessage = pMessage;
}

FString UPacManGameInstance::Check_Message()
{
	return sMessage;
}

void UPacManGameInstance::Set_EatenFood(int32 pEatenFood)
{
	xEatenFood = xEatenFood + 1;
}

void UPacManGameInstance::Reset_EatenFood()
{
	xEatenFood = 0;
}

int32 UPacManGameInstance::Check_EatenFood()
{
	return xEatenFood;
}

