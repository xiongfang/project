﻿#pragma once

#include "GameBattler.generated.h"

class USkill;
class UState;

UENUM()
enum class DamageFlag :uint8
{
	Hit,
	Miss,
	Critical
};

UCLASS(abstract)
class AGameBattler :public ACharacter
{
	GENERATED_BODY()

private:
	void TriggerEnterCombating();
	void CheckLeaveCombating(float DeltaTime);


	bool NeedFaceToTarget = false;
	void UpdateFaceToTarget(float DeltaTime);

protected:
	virtual void NotifyEnterCombating(){}
	virtual void NotifyLeaveCombating(){}
public:
	AGameBattler();

	UENUM()
	enum class CampType :uint8
	{
		Party,
		Enemy
	};

	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
	float face_rotate_speed = 360.0f;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
		CampType camp;

	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadOnly)
		int32 hp;
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
		int32 mp;

	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
		bool immortal;

	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadOnly)
		bool combating;

	UFUNCTION(Category = Logic, BlueprintCallable)
		bool IsEnemy(AGameBattler* battler){ check(battler);  return battler->camp != camp; }

	UFUNCTION(Category = Logic, BlueprintCallable)
		bool IsDead(){ return hp <= 0 && !immortal; }

		float timer_combat_cd;

		virtual int32 base_maxhp(){ return 0; }

		virtual int32 base_maxmp(){ return 0; }

		virtual int32 base_patk(){ return 0; }

		virtual int32 base_matk(){ return 0; }

		virtual int32 base_pdef(){ return 0; }

		virtual int32 base_mdef(){ return 0; }
		virtual int32 base_hit(){ return 0; }
		virtual int32 base_eva(){ return 0; }

		virtual USoundBase* dead_sound(){ return NULL;}

		UFUNCTION(Category = Logic, BlueprintCallable)
			virtual FName name(){ return NAME_None; }

		UFUNCTION(Category = Logic, BlueprintCallable)
			virtual int32 maxhp();
		UFUNCTION(Category = Logic, BlueprintCallable)
			virtual int32 maxmp();
		UFUNCTION(Category = Logic, BlueprintCallable)
			virtual int32 patk();
		UFUNCTION(Category = Logic, BlueprintCallable)
			virtual int32 matk();
		UFUNCTION(Category = Logic, BlueprintCallable)
			virtual int32 pdef();
		UFUNCTION(Category = Logic, BlueprintCallable)
			virtual int32 mdef();
		UFUNCTION(Category = Logic, BlueprintCallable)
			virtual int32 hit();
		UFUNCTION(Category = Logic, BlueprintCallable)
			virtual int32 eva();

		UFUNCTION(Category = Logic, BlueprintCallable)
			virtual FName race(){ return NAME_None; }

		UFUNCTION(Category = Logic, BlueprintCallable)
			int32 damage(AGameBattler* User,DamageFlag dt, int32 hp_damage, int32 mp_damage);


	UPROPERTY(Category = Data, VisibleAnywhere)
		TMap<FName, USkill*> skills;
	UPROPERTY(Category = Data, VisibleAnywhere)
		TMap<FName, UState*> states;

	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		AGameBattler* Target;  //当前选择的目标

	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		float skill_common_cd; //公共CD
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		USkill* current_skill;//当前正在使用的技能

	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual bool can_move();
	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual bool can_use_skill(USkill* skill);
	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual bool can_use_skill_target(USkill* skill,AGameBattler* bt);

	UFUNCTION(Category = Logic, BlueprintCallable)
		void LearnSkill(FName skillId);

	UFUNCTION(Category = Logic, BlueprintCallable)
		TArray<USkill*> GetSkills();

	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual void SkillEffect(AGameBattler* User, USkill* skill);

	//UFUNCTION(BlueprintImplementableEvent)
	//	TArray<AGameBattler*> ReceiveSkillGetTargets(USkill* skill);
	//UFUNCTION(BlueprintImplementableEvent)
	//	void ReceiveSkillEffect(AGameBattler* User, USkill* skill);

	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual bool Attack(FName skillId);


	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual void SelectTarget(AGameBattler* t);


	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual void Recover();


	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual void AnimNofity_SkillEffect();
	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual void AnimNofity_Shoot();

	//状态
	UFUNCTION(Category = Logic, BlueprintCallable)
		void AddState(FName stateId);
	UFUNCTION(Category = Logic, BlueprintCallable)
		void RemoveState(FName stateId);

	UFUNCTION(BlueprintImplementableEvent)
		void ShowDamageUI(const FString& dmg, const FColor& color=FColor::White);




	virtual void Event_OnHit_Implementation(AGameBattler* User, USkill* skill){}
	virtual void Event_OnSelect_Implementation(AGameBattler* User){}
	virtual void Event_OnDead_Implementation(AGameBattler* User);

	UFUNCTION(BlueprintNativeEvent)
		void Event_OnHit(AGameBattler* User,USkill* skill);
	UFUNCTION(BlueprintNativeEvent)
		void Event_OnSelect(AGameBattler* User);
	UFUNCTION(BlueprintNativeEvent)
		void Event_OnDead(AGameBattler* User);

	//查找指定半径的战斗者，并按距离从近到远排列
	UFUNCTION(Category = Logic, BlueprintCallable)
	TArray<AGameBattler*> FindBattlers(float Radius);

	virtual void SerializeProperty(FArchive& ar);
};
