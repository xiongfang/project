// Fill out your copyright notice in the Description page of Project Settings.

#include "client.h"
#include "GameMonster.h"
#include "MyGameSingleton.h"
#include "config.h"
#include "Monster_AIController.h"
#include "ItemActor.h"

// Sets default values
AGameMonster::AGameMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	camp = CampType::Enemy; //默认为敌人阵营
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
int32 AGameMonster::base_hit()
{
	return GetData()->hit;
}
int32 AGameMonster::base_eva()
{
	return GetData()->eva;
}

USoundBase* AGameMonster::dead_sound()
{
	return GetData()->dead_sound.LoadSynchronous();
}
FName AGameMonster::race()
{
	return GetData()->race;
}
float AGameMonster::attention_range()
{ return GetData()->attention_range; }

void AGameMonster::InitCreate(FName id)
{
	this->id = id;

	//创建所有身体
	Fconfig_monster* monsterData = UMyGameSingleton::Get().FindMonster(id);
	check(monsterData != NULL);

	//加载资源
	//monsterData->model.ToStringReference().TryLoad();
	//GetMesh()->SetSkeletalMesh(monsterData->model.Get());

	if (UMyGameSingleton::Get().FindSkill(monsterData->skill1) != NULL)
	{
		LearnSkill(monsterData->skill1);
	}
	if (UMyGameSingleton::Get().FindSkill(monsterData->skill2) != NULL)
	{
		LearnSkill(monsterData->skill2);
	}
}


void AGameMonster::Event_OnHit_Implementation(AGameBattler* User, USkill* skill)
{
	AMonster_AIController* controller = Cast<AMonster_AIController>(GetController());
	if (controller != NULL)
	{
		controller->OnHit(User, skill);
	}
}

void AGameMonster::Drop()
{
	TMap<FName,int32> items;
	Fconfig_monster* data = GetData();
	for (auto drop : data->drops)
	{
		Fconfig_item* item = UMyGameSingleton::Get().FindItem(drop.name);
		if (item != NULL && FMath::FRand() <= drop.rate)
		{
			items.Add(drop.name, drop.num);
		}
	}
	if (items.Num() > 0)
	{
		FVector randOffset;
		randOffset.X = FMath::FRand();
		randOffset.Y = FMath::FRand();
		randOffset *= 100.0f;

		AItemActor* actor = GetWorld()->SpawnActor<AItemActor>(UMyGameSingleton::Get().item_actor_class, GetActorLocation() + randOffset, FRotator::ZeroRotator);
		actor->items.Append(items);
	}
}

void AGameMonster::Event_OnDead_Implementation()
{
	Super::Event_OnDead_Implementation();
	Drop();
}