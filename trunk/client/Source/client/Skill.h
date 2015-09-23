#pragma once
#include "MyGameSingleton.h"
#include "Skill.generated.h"



class AGameBattler;


UCLASS(Blueprintable)
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

	TArray<AGameBattler*> ReceiveSkillGetTargets_Implementation(AGameBattler* User, USkill* skill);
	void ReceiveSkillEffect_Implementation(AGameBattler* Target, AGameBattler* User, USkill* skill);

	UFUNCTION(BlueprintNativeEvent)
		TArray<AGameBattler*> ReceiveSkillGetTargets(AGameBattler* User,USkill* skill);
	UFUNCTION(BlueprintNativeEvent)
		void ReceiveSkillEffect(AGameBattler* Target,AGameBattler* User, USkill* skill);
};

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

UCLASS()
class UState :public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FName id;

	UPROPERTY()
	AActor* PS;

	float cd;

	Fconfig_state* GetData();

	
};



UCLASS()
class UTask :public UObject
{
	GENERATED_BODY()
public:
	UENUM()
	enum class TaskState :uint8
	{
		GOING,
		FINISHED
	};

	UPROPERTY()
	FName id;

	UPROPERTY()
	TaskState State;

	Fconfig_task* GetData();

	
};