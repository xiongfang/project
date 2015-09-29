// Fill out your copyright notice in the Description page of Project Settings.

#include "client.h"
#include "MyPlayerController.h"
#include "GameBattler.h"

AMyPlayerController::AMyPlayerController()
{
	Battler = NULL;
}

void AMyPlayerController::SetPawn(APawn* pawn)
{
	Super::SetPawn(pawn);

	Battler = Cast<AGameBattler>(pawn);
	if (pawn != NULL)
	{
		check(Battler);
	}
}
// Called when the game starts or when spawned
void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	bool FingerPressed[2];

	FVector2D NewLocation[2];

	GetInputTouchState(ETouchIndex::Touch1, NewLocation[0].X, NewLocation[0].Y, FingerPressed[0]);
	GetInputTouchState(ETouchIndex::Touch2, NewLocation[1].X, NewLocation[1].Y, FingerPressed[1]);
	FVector hitPos = FVector::ZeroVector;

	if (FingerPressed[0] && !FingerPressed[1])
	{
		FVector Pos, Dir;
		if (DeprojectScreenPositionToWorld(NewLocation[0].X, NewLocation[0].Y, Pos, Dir))
		{
			FHitResult HitInfo;
			FCollisionQueryParams QParams;
			FCollisionObjectQueryParams OParams;
			if (GetWorld()->LineTraceSingleByObjectType(HitInfo, Pos, Pos + Dir * 10240.f, OParams, QParams))
			{
				//DrawDebugSphere(GetWorld(), HitInfo.ImpactPoint, 10, 10, FColor::Red, false, 1);

				AGameBattler* battler = Cast<AGameBattler>(HitInfo.Actor.Get());
				if (battler != NULL)
				{
					Battler->Target = battler;
					battler->Event_OnSelect(this->Battler);
				}
			}
			else
			{
			}

		}
	}
	
}