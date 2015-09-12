// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Character.h"
#include "config.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct CLIENT_API Fconfig_item : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName show_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 type;
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


	enum ItemType
	{
		Money,
		Item,
		Equip
	};
};


USTRUCT(BlueprintType)
struct CLIENT_API Fconfig_race : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString show_name;
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
struct CLIENT_API Fconfig_equip : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 slot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 patk_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 matk_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 hp_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 pdef_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 mdef_plus;

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
		FName anim_group;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimMontage> open_weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimMontage> close_weapon;
};
USTRUCT(BlueprintType)
struct CLIENT_API Fconfig_skill : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName show_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float cd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float patk_percent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 patk_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float matk_percent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 matk_plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float target_maxhp_percent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 mp_cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 sp_cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float interrupt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 hit_recover;

};

USTRUCT(BlueprintType)
struct CLIENT_API Fconfig_effect : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 valid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimMontage> start_self_anim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimMontage> start_weapon_anim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UStaticMesh> fly_body;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimMontage> hit_anim;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimSequence> attack_left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimSequence> attack_right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TAssetPtr<UAnimSequence> attack_power;
};
