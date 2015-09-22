#pragma once

#include "GameBattler.generated.h"

class USkill;

UCLASS(abstract)
class AGameBattler :public ACharacter
{
	GENERATED_BODY()

public:
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


	UFUNCTION(Category = Logic, BlueprintCallable)
		virtual void SkillEffect(AGameBattler* User, USkill* skill);

	UFUNCTION(BlueprintImplementableEvent)
		void ReceiveSkillEffect(AGameBattler* User, USkill* skill);
};
