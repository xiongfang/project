#pragma once

#include "GameBattler.generated.h"

class USkill;

UCLASS(abstract)
class AGameBattler :public ACharacter
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;


	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
		int32 hp;
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
		int32 mp;
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
		FName race;

		virtual int32 base_maxhp(){ return 0; }

		virtual int32 base_maxmp(){ return 0; }

		virtual int32 base_patk(){ return 0; }

		virtual int32 base_matk(){ return 0; }

		virtual int32 base_pdef(){ return 0; }

		virtual int32 base_mdef(){ return 0; }


	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual int32 maxhp(){ return base_maxhp(); }
	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual int32 maxmp(){ return base_maxmp(); }
	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual int32 patk(){ return base_patk(); }
	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual int32 matk(){ return base_matk(); }
	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual int32 pdef(){ return base_pdef(); }
	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual int32 mdef(){ return base_mdef(); }

	UPROPERTY(Category = Data, VisibleAnywhere)
		TMap<FName, USkill*> skills;

	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		AGameBattler* Target;  //当前选择的目标


	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		float skill_common_cd; //公共CD
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		USkill* current_skill;//当前正在使用的技能

	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual bool can_move();
	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual bool can_use_skill();

	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual bool can_use_skill_target(FName skill);

	UFUNCTION(Category = Logic, BlueprintCallable)
		void LearnSkill(FName skillId);

	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual void SkillEffect(AGameBattler* User, USkill* skill);

	UFUNCTION(BlueprintImplementableEvent)
		void ReceiveSkillEffect(AGameBattler* User, USkill* skill);

	UFUNCTION(Category = Logic, BlueprintCallable)
		void Attack(FName skillId);


	UFUNCTION(Category = Logic, BlueprintCallable)
		void SelectTarget(AGameBattler* t);


	UFUNCTION(Category = Logic, BlueprintCallable)
		void Recover();


	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual void AnimNofity_SkillEffect();
	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual void AnimNofity_Shoot();
};
