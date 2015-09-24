// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Character.h"
#include "GameMonster.h"

#include "config.generated.h"
/**
 * 
 */

UENUM()
enum class ItemType :uint8
{
	Money,
	Item,
	Equip
};


USTRUCT(BlueprintType)
struct CLIENT_API Fconfig_item : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UTexture> icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ItemType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName child_key;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 arg1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 arg2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 arg3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString arg4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 max;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 can_sell;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 can_use;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString script;

	
};


USTRUCT(BlueprintType)
struct CLIENT_API Fconfig_race : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 sex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ACharacter> character;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName anim_group;
};

USTRUCT(BlueprintType)
struct CLIENT_API Fconfig_class : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString description;
};


USTRUCT(BlueprintType)
struct CLIENT_API Fconfig_equip : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FName> class_type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 slot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 patk_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 matk_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 hp_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 mp_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 pdef_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 mdef_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 hit_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 eva_plus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 double_hand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 weapon_type;

	enum EquipPos
	{
		Head,
		Body,
		Hand,
		Feet,
		MainHand,
		SecondHand
	};

	enum WeaponType
	{
		Sword,
		LongSword,
		Bow,
		Shield,
		Count
	};
};


USTRUCT(BlueprintType)
struct CLIENT_API Fconfig_armor_map : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<USkeletalMesh> model;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<USkeletalMesh> append_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<USkeletalMesh> append_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 replace;
};



USTRUCT(BlueprintType)
struct CLIENT_API Fconfig_weapon_map : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	FName anim_group;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UStaticMesh> model;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UStaticMesh> append_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UStaticMesh> append_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<USkeletalMesh> append_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName anim_group;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimMontage> open_weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimMontage> close_weapon;
};

USTRUCT()
struct FStateChange
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float rate;
};

UENUM()
enum class SkillSelectTargetType:uint8
{
	Battler,
	Position
};
UENUM()
enum class SkillEffectTargetType :uint8
{
	Enemy,
	Friend,
	Self
};

UENUM()
enum class SkillRangeType :uint8
{
	None,
	Self,
	Target
};
UENUM()
enum class SkillDamageType :uint8
{
	Physic, //物理
	Magic	//魔法
};


USTRUCT(BlueprintType)
struct CLIENT_API Fconfig_skill : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UTexture> icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float cd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float common_cd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		SkillSelectTargetType select_type; //选择目标类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		SkillEffectTargetType target_type; //目标类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		SkillRangeType range_type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		SkillDamageType damage_type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool must_hit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ignoring_defense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float atk_percent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float atk_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 cost_mp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float distance; //释放距离
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FStateChange> state_plus; //附加技能
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FStateChange> state_minus; //移除技能
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<USkill> prefab; //重写的技能
};

USTRUCT(BlueprintType)
struct CLIENT_API Fconfig_effect : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* start_self_anim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* start_weapon_anim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* start_target_anim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool fly_all_target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> fly_body;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector fly_offset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fly_speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* fly_explosion_fx;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* hit_anim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* hit_fx;
};
USTRUCT(BlueprintType)
struct CLIENT_API Fconfig_anim_group : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UBlendSpace> movement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimMontage> block_hit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimSequence> block_idle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimMontage> block_bash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimSequence> jump_start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimSequence> jump_fall;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimSequence> jump_land;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimSequence> staggerbacksmall;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimSequence> staggerbacklarge;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TAssetPtr<UAnimSequence> staggerbacklargest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimSequence> staggerforwardsmall;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimSequence> staggerforwardlarge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimSequence> staggerforwardlargest;
};

USTRUCT(BlueprintType)
struct CLIENT_API Fconfig_monster : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName race;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TAssetPtr<USkeletalMesh> model;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TAssetPtr<UBlendSpace> movement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName skill1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName skill2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 maxhp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 maxmp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 patk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 matk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 pdef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 mdef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 hit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 eva;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AGameMonster> prefab;
};


USTRUCT(BlueprintType)
struct CLIENT_API Fconfig_state : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UTexture> icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UParticleSystem> ps;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float patk_percent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 patk_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float matk_percent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 matk_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float hp_percent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 hp_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 mp_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 pdef_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 mdef_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 hit_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 eva_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool cant_move;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool cant_use_skill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FName> state_minus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float shock_release_prob;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float time_schedule;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UState> prefab;
};


USTRUCT()
struct FItem
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Num;
};

USTRUCT(BlueprintType)
struct CLIENT_API Fconfig_task : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItem> rewards;
};