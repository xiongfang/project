#pragma once
#include "MyGameSingleton.h"
#include "Skill.generated.h"



class AGameBattler;


UCLASS(Blueprintable)
class USkill :public UObject
{
	GENERATED_BODY()


public:
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
	FName id;
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
	int32 level;
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
	float cd;


	Fconfig_skill* GetData();
	
	void Update(float deltaTime);

	virtual TArray<AGameBattler*> GetTargets_Implementation(AGameBattler* User);
	virtual void SkillEffect_Implementation(AGameBattler* Target, AGameBattler* User);

	UFUNCTION(BlueprintNativeEvent)
		TArray<AGameBattler*> GetTargets(AGameBattler* User);
	UFUNCTION(BlueprintNativeEvent)
		void SkillEffect(AGameBattler* Target, AGameBattler* User);

	bool can_use(){ return cd <= 0; }
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

UCLASS(Blueprintable)
class UState :public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FName id;

	UPROPERTY()
	UParticleSystemComponent* PS;

	float cd;
	float schedule_timer;

	Fconfig_state* GetData();


	UFUNCTION(BlueprintImplementableEvent)
		void ReceiveTimeSchedule(AGameBattler* User);
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

	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
	FName id;

	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
	TaskState State;

	Fconfig_task* GetData();
};