// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "Game.generated.h"

class AGameCharacter;
class UMySaveGame;
/**
 * 
 */
UCLASS()
class CLIENT_API UGame : public UGameInstance
{
	GENERATED_BODY()

	UPROPERTY()
	UMySaveGame* TempSavedGame;
public:

	void Save(AGameCharacter* character, TArray<uint8>& data);
	void Load(AGameCharacter* character, TArray<uint8>& data);

	UFUNCTION(Category = Logic, BlueprintCallable)
	void AutoSaveGameCharacter(AGameCharacter* character);
	UFUNCTION(Category = Logic, BlueprintCallable)
	void AutoLoadGameCharacter(AGameCharacter* character);
};
