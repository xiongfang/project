// Fill out your copyright notice in the Description page of Project Settings.

#include "client.h"
#include "My3DWidgetComponent.h"


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
			FVector playerPos = TargetPlayer->PlayerController->GetControlledPawn()->GetActorLocation();
			bool show = FVector::Dist(playerPos, GetOwner()->GetActorLocation()) < show_distance;
			this->SetVisibility(show);
		}
	}
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}