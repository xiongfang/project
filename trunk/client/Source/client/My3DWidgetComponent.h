// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/WidgetComponent.h"
#include "My3DWidgetComponent.generated.h"

/**
 * 
 */
UCLASS(hidecategories = (Object, Activation, "Components|Activation", Sockets, Base, Lighting, LOD, Mesh), editinlinenew, meta = (BlueprintSpawnableComponent))
class CLIENT_API UMy3DWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	float show_distance;

	UMy3DWidgetComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};
