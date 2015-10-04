// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class AGameBattler;
class USkill;
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
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused)override;
	
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
	float client_camera_scale_min;
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
	float client_camera_scale_max;
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
	float client_camera_scale_speed;
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
	float attention_range;


	UFUNCTION(Category = Logic, BlueprintCallable)
	bool AutoSelectTarget(USkill* skill);

	//Ïà»ú¿ØÖÆ
	USpringArmComponent* SprintArm;
	FVector2D oldTouchPositions[2];
	FVector2D oldTouchVector;
	bool fingerState[2];
	float oldTouchDistance;
};
