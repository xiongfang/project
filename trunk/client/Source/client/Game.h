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


public:
	UGame();
	virtual void Init()override;
	virtual void Shutdown()override;

	UPROPERTY(BlueprintReadOnly)
	UMySaveGame* TempSavedGame;

	
	void AutoSaveGameCharacter();
	bool LoadFromSlot;

	UFUNCTION(Category = Logic, BlueprintCallable)
	APawn* LoadOrCreateCharacter(TSubclassOf<class APawn> pwanClass,AController* NewPlayer, class AActor* StartSpot);


	UFUNCTION(Category = Logic, BlueprintCallable)
		void SaveSlot(int32 slot);
	UFUNCTION(Category = Logic, BlueprintCallable)
		void LoadSlot(int32 slot);

	void OnPreLoadMap();
	void OnPostLoadMap();

	UFUNCTION(Category = Logic, BlueprintCallable)
		void CapetureScreenShot();

	UFUNCTION(Category = Logic, BlueprintCallable)
		TArray<UMySaveGame*> LoadAllSaved();
};
