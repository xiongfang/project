// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

class AGameCharacter;

UCLASS()
class CLIENT_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TMap<FName,int32> items;

	UFUNCTION(Category = Logic, BlueprintCallable)
	void Pickup(AGameCharacter* character);
};
