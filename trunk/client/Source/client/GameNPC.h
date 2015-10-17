// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameMonster.h"
#include "GameNPC.generated.h"

struct FSentence;
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

		UPROPERTY()
		float talk_range;
		UPROPERTY()
		UTask* current_task;
		UPROPERTY()
		AGameCharacter* character;
		UPROPERTY()
		int32 sentence_index;

	UFUNCTION(Category = Logic, BlueprintCallable)
	TArray<UTask*> GetTasks(AGameCharacter* c);


	virtual void Event_OnSelect_Implementation(AGameBattler* User)override;


	UFUNCTION(Category = Logic, BlueprintCallable)
		void OnDialogSelect(const FString& token, int32 index);
};
