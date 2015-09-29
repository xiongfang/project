// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class AGameBattler;
/**
 * 
 */
UCLASS()
class CLIENT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

	AGameBattler* Battler;
public:
	AMyPlayerController();
	virtual void SetPawn(APawn* pawn)override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	
};
