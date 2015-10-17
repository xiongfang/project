// Fill out your copyright notice in the Description page of Project Settings.

#include "client.h"
#include "ItemActor.h"
#include "GameCharacter.h"

// Sets default values
AItemActor::AItemActor()
{
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}


void AItemActor::Pickup(AGameCharacter* character)
{
	for (auto item : items)
	{
		character->ItemAdd(item.Key, item.Value);
	}
	items.Empty();
	Destroy();
}