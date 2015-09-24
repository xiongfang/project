// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Animation/SkeletalMeshActor.h"
#include "MyGameSingleton.generated.h"


struct Fconfig_armor_map;
struct Fconfig_weapon_map;
struct Fconfig_race;
struct Fconfig_item;
struct Fconfig_equip;
struct Fconfig_anim_group;
struct Fconfig_skill;
struct Fconfig_class;
struct Fconfig_effect;
struct Fconfig_monster;
struct Fconfig_state;
struct Fconfig_task;
/**
 * 
 */
UCLASS(Blueprintable)
class CLIENT_API UMyGameSingleton : public UObject, public FTickerObjectBase
{
	GENERATED_BODY()
public:
	UMyGameSingleton();
	~UMyGameSingleton();


	static UMyGameSingleton& Get(); 	// Get method to access this object
	//FStreamableManager AssetLoader;		// Your asset loader

	// Override from FTicker::Tick()
	// if you forget to Override this and implement on cpp file you'll get Error 2259
	virtual bool Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UDataTable* config_race;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* config_item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* config_equip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* config_anim_group;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* config_armor_map;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* config_weapon_map;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* config_skill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* config_class;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* config_effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* config_monster;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* config_state;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* config_task;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	UCurveTable* config_curve_hp;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	UCurveTable* config_curve_mp;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	UCurveTable* config_curve_patk;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	UCurveTable* config_curve_matk;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	UCurveTable* config_curve_pdef;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	UCurveTable* config_curve_mdef;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	UCurveTable* config_curve_hit;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	UCurveTable* config_curve_eva;

	static FName MakeKey(FName key1, FName key2);

	Fconfig_armor_map* FindArmorMap(FName equipId, FName race);
	Fconfig_weapon_map* FindWeaponMap(FName equipId, FName race);
	Fconfig_effect* FindEffect(FName skillId, FName race);

	Fconfig_race* FindRace(FName raceId);
	Fconfig_item* FindItem(FName itemId);
	Fconfig_equip* FindEquip(FName itemId);
	Fconfig_anim_group* FindAnimGroup(FName id);
	Fconfig_skill* FindSkill(FName skillId);
	Fconfig_class* FindClass(FName id);
	Fconfig_monster* FindMonster(FName id);
	Fconfig_state* FindState(FName id);
	Fconfig_task* FindTask(FName id);
};
