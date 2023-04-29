// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PacManGameInstance.generated.h"

UCLASS()
class PACMAN_API UPacManGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	bool bExec = false;

	bool bIsFR = false;
	bool bIsCH = true;

	bool bPNHomeB = false;
	bool bPNHomeC = false;
	bool bPNHomeI = false;
	bool bPNHomeP = false;

	bool bPNCanExitHomeB = false;
	bool bPNCanExitHomeC = false;
	bool bPNCanExitHomeI = false;
	bool bPNCanExitHomeP = false;

	UPROPERTY(EditAnywhere)
	int32 xScore = 0;
	int32 xLives = 3;
	int32 xEatenFood = 0;

	FString sMessage;

	UFUNCTION(BlueprintCallable)
	void Set_FR(bool pIsFR);
	UFUNCTION(BlueprintCallable)
	bool Check_FR();

	UFUNCTION(BlueprintCallable)
	void Set_Exec(bool pExec);
	UFUNCTION(BlueprintCallable)
	bool Check_Exec();

	void Increase_Score(int32 pScore);

	UFUNCTION(BlueprintCallable)
	void Set_Score(int pScore);

	UFUNCTION(BlueprintCallable)
	int Check_Score();

	void Decrease_Lives();

	UFUNCTION(BlueprintCallable)
	void Set_Lives(int32 pLives);

	UFUNCTION(BlueprintCallable)
	int32 Check_Lives();

	void Set_PhantomGoHome(bool PNHome, class AGridPawn* PN);

	bool Check_PhantomGoHome(class AGridPawn* PN);

	UFUNCTION(BlueprintCallable)
	void Set_AllPhantomHome(bool PNHome);

	UFUNCTION(BlueprintCallable)
	void Set_PhantomExitHome(bool PNCanExitHome, AGridPawn* PN);

	UFUNCTION(BlueprintCallable)
	bool Check_PhantomExitHome(AGridPawn* PN);

	FString Check_EPhantomName(AGridPawn* PN);

	UFUNCTION(BlueprintCallable)
	void Set_Message(FString pMessage);

	UFUNCTION(BlueprintCallable)
	FString Check_Message();

	UFUNCTION(BlueprintCallable)
	void Set_EatenFood(int32 pEatenFood);
	UFUNCTION(BlueprintCallable)
	void Reset_EatenFood();
	UFUNCTION(BlueprintCallable)
	int32 Check_EatenFood();







};
