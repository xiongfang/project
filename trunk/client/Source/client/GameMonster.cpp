// Fill out your copyright notice in the Description page of Project Settings.

#include "client.h"
#include "GameMonster.h"
#include "MyGameSingleton.h"
#include "config.h"

// Sets default values
AGameMonster::AGameMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameMonster::BeginPlay()
{
	InitCreate(id);

	Super::BeginPlay();
	
}

// Called every frame
void AGameMonster::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AGameMonster::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}



Fconfig_monster* AGameMonster::GetData()
{
	Fconfig_monster* monsterData = UMyGameSingleton::Get().FindMonster(id);
	check(monsterData != NULL);
	return monsterData;
}

int32 AGameMonster::base_maxhp()
{
	return GetData()->maxhp;
}
int32 AGameMonster::base_maxmp()
{
	return GetData()->maxmp;
}
int32 AGameMonster::base_patk()
{
	return GetData()->patk;
}
int32 AGameMonster::base_matk()
{
	return GetData()->matk;
}
int32 AGameMonster::base_pdef()
{
	return GetData()->pdef;
}
int32 AGameMonster::base_mdef()
{
	return GetData()->mdef;
}

void AGameMonster::InitCreate(FName id)
{
	this->id = id;
	this->race = GetData()->race;

	//创建所有身体
	Fconfig_monster* monsterData = UMyGameSingleton::Get().FindMonster(id);
	check(monsterData != NULL);

	//加载资源
	monsterData->model.ToStringReference().TryLoad();
	GetMesh()->SetSkeletalMesh(monsterData->model.Get());
}