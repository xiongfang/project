// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameMonster.h"
#include "GameNPC.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API AGameNPC : public AGameMonster
{
	GENERATED_BODY()
	
public:
	AGameNPC();

	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* Head;
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* Body;
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* Hand;
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* Feet;
	
	
};
