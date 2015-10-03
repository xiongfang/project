// Fill out your copyright notice in the Description page of Project Settings.

#include "client.h"
#include "My3DWidgetComponent.h"
#include "GameBattler.h"

UMy3DWidgetComponent::UMy3DWidgetComponent()
{
	show_distance = 2000.0f;
}

void UMy3DWidgetComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	//¼ÆËã¾àÀë
	UWorld* SafeWorld = GetWorld();
	if (SafeWorld)
	{
		ULocalPlayer* TargetPlayer = GetOwnerPlayer();

		if (TargetPlayer != NULL && TargetPlayer->PlayerController!=NULL)
		{
			AGameBattler* battler = Cast<AGameBattler>(TargetPlayer->PlayerController->GetPawn());
			if (battler != NULL)
			{
				FVector playerPos = battler->GetActorLocation();
				bool show = FVector::Dist(playerPos, GetOwner()->GetActorLocation()) < show_distance && !battler->IsDead();
				this->SetVisibility(show);
			}
			
		}
	}
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}