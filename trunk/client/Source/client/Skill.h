#pragma once
#include "MyGameSingleton.h"
#include "Skill.generated.h"

UCLASS()
class USkill :public UObject
{
	GENERATED_BODY()


public:
	UPROPERTY()
	FName id;
	UPROPERTY()
	int32 level;

	float cd;

	Fconfig_skill* GetData();
	
	void Update(float deltaTime);

};


class AGameBattler;

UCLASS()
class UProjectile :public UActorComponent
{
	GENERATED_BODY()
public:
	UProjectile();

	UPROPERTY()
	USkill* skill;

	UPROPERTY()
	AGameBattler* Owner;
	UPROPERTY()
	AGameBattler* Target;

	UPROPERTY()
	FVector TargetPosition;

	float speed; 

	void InitCreate(AGameBattler* o, AGameBattler* t, USkill* s);

	void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)override;

};