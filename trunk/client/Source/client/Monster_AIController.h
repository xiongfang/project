// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Monster_AIController.generated.h"

class AGameMonster;

/**
 * 
 */
UCLASS()
class CLIENT_API AMonster_AIController : public AAIController
{
	GENERATED_BODY()

protected:
	AGameMonster* Monster;

	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	float ai_tick_period;

	float ai_tick_timer;
	void TickAI(float DeltaSeconds);

	FVector home_location;

public:
	AMonster_AIController();

	virtual void SetPawn(APawn* pawn)override;
	AGameMonster* GetMonster(){ return Monster; }


	virtual void Tick(float DeltaSeconds) override;


	UFUNCTION(Category = Logic, BlueprintCallable)
		TArray<AGameBattler*> GetAttentionBattlers();

	AGameBattler* SelectTarget_Implementation(const TArray<AGameBattler*>& battlers);

	UFUNCTION(BlueprintNativeEvent)
		AGameBattler* SelectTarget(const TArray<AGameBattler*>& battlers);
};
