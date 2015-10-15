#pragma once

#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"


class UStaticMeshComponent;
class USkeletalMeshComponent;

UCLASS()
class CLIENT_API AWeaponActor : public AActor
{
	GENERATED_BODY()
public:
	UStaticMeshComponent* GetStaticMeshComponent();
	USkeletalMeshComponent* GetSkeletalMeshComponent();
};