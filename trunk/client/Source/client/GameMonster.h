// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameBattler.h"
#include "GameMonster.generated.h"

struct Fconfig_monster;

UCLASS()
class CLIENT_API AGameMonster : public AGameBattler
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameMonster();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	FName id;

	Fconfig_monster* GetData();

	int32 base_maxhp() override;
	int32 base_maxmp() override;
	int32 base_patk() override;
	int32 base_matk() override;
	int32 base_pdef() override;
	int32 base_mdef() override;

	UFUNCTION(Category = Logic, BlueprintCallable)
		void InitCreate(FName monsterId);
};
