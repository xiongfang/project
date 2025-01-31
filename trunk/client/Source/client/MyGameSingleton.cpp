// Fill out your copyright notice in the Description page of Project Settings.

#include "client.h"
#include "MyGameSingleton.h"
#include "config.h"

const FString ContextString = "MyCharacter";


UMyGameSingleton::UMyGameSingleton()
{
	//ConstructorHelpers::FObjectFinder<UDataTable> armor_map(TEXT("/Game/data/config_armor_map"));
	//ConstructorHelpers::FObjectFinder<UDataTable> race(TEXT("/Game/data/config_race"));
	
}

UMyGameSingleton::~UMyGameSingleton()
{
}

//Get method to access the instance
UMyGameSingleton& UMyGameSingleton::Get()
{
	UMyGameSingleton *Singleton = Cast<UMyGameSingleton>(GEngine->GameSingleton);
	return *Singleton;
}

// OVERRIDE from FTICKER TICK (Preventing Error 2259)
bool UMyGameSingleton::Tick(float DeltaSeconds)
{
	return true;
}


FName UMyGameSingleton::MakeKey(FName key1, FName key2)
{
	return *(key1.ToString() + "_" + key2.ToString());
}

Fconfig_armor_map* UMyGameSingleton::FindArmorMap(FName equipId, FName race)
{

	return config_armor_map->FindRow<Fconfig_armor_map>(MakeKey(race,equipId), ContextString,false);
	
}


Fconfig_weapon_map*  UMyGameSingleton::FindWeaponMap(FName equipId, FName race)
{
	return config_weapon_map->FindRow<Fconfig_weapon_map>(MakeKey(race, equipId), ContextString, false);
}
Fconfig_effect* UMyGameSingleton::FindEffect(FName skillId, FName race)
{
	return config_effect->FindRow<Fconfig_effect>(MakeKey(race, skillId), ContextString, false);
}

Fconfig_race* UMyGameSingleton::FindRace(FName raceId)
{
	return config_race->FindRow<Fconfig_race>(raceId, ContextString, false);
}
Fconfig_item* UMyGameSingleton::FindItem(FName itemId)
{
	return config_item->FindRow<Fconfig_item>(itemId, ContextString, false);
}
Fconfig_equip* UMyGameSingleton::FindEquip(FName itemId)
{
	Fconfig_item* item = FindItem(itemId);
	if (item != NULL && item->type == ItemType::Equip)
	{
		return config_equip->FindRow<Fconfig_equip>(item->child_key, ContextString, false);
	}
	return NULL;
}

Fconfig_anim_group* UMyGameSingleton::FindAnimGroup(FName id)
{
	return config_anim_group->FindRow<Fconfig_anim_group>(id, ContextString, false);
}

Fconfig_skill* UMyGameSingleton::FindSkill(FName skillId)
{
	return config_skill->FindRow<Fconfig_skill>(skillId, ContextString, false);
}

Fconfig_class* UMyGameSingleton::FindClass(FName id)
{
	return config_class->FindRow<Fconfig_class>(id, ContextString, false);
}

Fconfig_monster* UMyGameSingleton::FindMonster(FName id)
{
	return config_monster->FindRow<Fconfig_monster>(id, ContextString, false);
}

Fconfig_state* UMyGameSingleton::FindState(FName id)
{
	return config_state->FindRow<Fconfig_state>(id, ContextString, false);
}

Fconfig_task* UMyGameSingleton::FindTask(FName id)
{
	return config_task->FindRow<Fconfig_task>(id, ContextString, false);
}

int32 UMyGameSingleton::GetLevelExp(int32 level)
{
	return config_exp->FindRow<Fconfig_exp>(*FString::Printf(TEXT("%d"),level), ContextString, false)->exp;
}