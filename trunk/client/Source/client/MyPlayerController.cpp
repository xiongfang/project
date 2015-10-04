// Fill out your copyright notice in the Description page of Project Settings.

#include "client.h"
#include "MyPlayerController.h"
#include "GameBattler.h"

AMyPlayerController::AMyPlayerController()
{
	Battler = NULL;

	client_camera_scale_min = 150.0f;
	client_camera_scale_max = 300.0f;
	client_camera_scale_speed = 100.0f;
	fingerState[0] = fingerState[1] = false;
}

void AMyPlayerController::SetPawn(APawn* pawn)
{
	Super::SetPawn(pawn);

	Battler = Cast<AGameBattler>(pawn);
	if (pawn != NULL)
	{
		check(Battler);
		SprintArm = Battler->FindComponentByClass<USpringArmComponent>();
	}
}
// Called when the game starts or when spawned
void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyPlayerController::PostProcessInput(const float DeltaSeconds, const bool bGamePaused)
{
	Super::PostProcessInput(DeltaSeconds, bGamePaused);

	if (Battler == NULL)
	{
		return;
	}

	//获得触屏状态
	bool FingerPressed[2];

	FVector2D NewLocation[2];

	GetInputTouchState(ETouchIndex::Touch1, NewLocation[0].X, NewLocation[0].Y, FingerPressed[0]);
	GetInputTouchState(ETouchIndex::Touch2, NewLocation[1].X, NewLocation[1].Y, FingerPressed[1]);



	//桌面平台鼠标控制
	{
		if (IsInputKeyDown(FKey("G")))
		{
			AddYawInput(90 * DeltaSeconds);
		}
		if (IsInputKeyDown(FKey("H")))
		{
			float scale = DeltaSeconds * 500;
			SprintArm->TargetArmLength += scale;
			SprintArm->TargetArmLength = FMath::Clamp(SprintArm->TargetArmLength, client_camera_scale_min, client_camera_scale_max);
		}
		if (IsInputKeyDown(FKey("J")))
		{
			float scale = DeltaSeconds * 500;
			SprintArm->TargetArmLength -= scale;
			SprintArm->TargetArmLength = FMath::Clamp(SprintArm->TargetArmLength, client_camera_scale_min, client_camera_scale_max);
		}
	}

	//旋转以及移动
	if (FingerPressed[0] && FingerPressed[1])
	{
		if (!fingerState[1]) {
			oldTouchPositions[0] = NewLocation[0];
			oldTouchPositions[1] = NewLocation[1];
			oldTouchVector = (oldTouchPositions[0] - oldTouchPositions[1]);
			oldTouchDistance = oldTouchVector.Size();

			fingerState[0] = fingerState[1] = true;
		}
		else
		{
			FVector2D newTouchVector = NewLocation[0] - NewLocation[1];
			float newTouchDistance = newTouchVector.Size();

			float rotate = FMath::Asin(FMath::Clamp((oldTouchVector.Y * newTouchVector.X - oldTouchVector.X * newTouchVector.Y) / oldTouchDistance / newTouchDistance, -1.0f, 1.0f)) / 0.0174532924f;

			/*FVector hitPos = FVector::ZeroVector;

			FVector Pos, Dir;
			if (Controller->DeprojectScreenPositionToWorld(0.5f, 0.5f, Pos, Dir))
			{
			FHitResult HitInfo;
			FCollisionQueryParams QParams;
			FCollisionObjectQueryParams OParams;
			if (GetWorld()->LineTraceSingle(HitInfo, Pos, Pos + Dir * 10240.f, QParams, OParams))
			{
			DrawDebugSphere(GetWorld(), HitInfo.ImpactPoint, 10, 10, FColor::Red, false, 1);

			}
			else
			{
			}

			}*/

			float scale = (newTouchDistance - oldTouchDistance)*DeltaSeconds*client_camera_scale_speed;
			SprintArm->TargetArmLength += scale;
			SprintArm->TargetArmLength = FMath::Clamp(SprintArm->TargetArmLength, client_camera_scale_min, client_camera_scale_max);

			AddYawInput(rotate);

			oldTouchPositions[0] = NewLocation[0];

			oldTouchPositions[1] = NewLocation[1];
			oldTouchVector = newTouchVector;
			oldTouchDistance = newTouchDistance;
		}

		return;
	}
	else
	{
		fingerState[0] = fingerState[1] = false;
	}

	//选择目标或者对象
	if (FingerPressed[0] && !FingerPressed[1] && fingerState[0] == false && fingerState[1]==false)
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
				else //移动到指定点
				{
					UNavigationSystem::SimpleMoveToLocation(this, HitInfo.ImpactPoint);
				}
			}
			else
			{
			}

		}
	}




	
}