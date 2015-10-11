// Fill out your copyright notice in the Description page of Project Settings.

#include "client.h"
#include "GameCharacter.h"
#include "MyGameSingleton.h"
#include "config.h"
#include "Weapon.h"

const FString ContextString = "MyCharacter";

DEFINE_LOG_CATEGORY(client);

#define WEAPON_SLOT_START  4

// Sets default values
AGameCharacter::AGameCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static FName CollisionProfileName(TEXT("IgnoreOnlyPawn"));

	Body = CreateOptionalDefaultSubobject< USkeletalMeshComponent>(TEXT("Body"));
	if (Body)
	{
		Body->AlwaysLoadOnClient = true;
		Body->AlwaysLoadOnServer = true;
		Body->bOwnerNoSee = false;
		Body->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
		Body->bCastDynamicShadow = true;
		Body->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Body->bChartDistanceFactor = true;
		Body->SetCollisionProfileName(CollisionProfileName);
		Body->bGenerateOverlapEvents = false;

		// Mesh acts as the head, as well as the parent for both animation and attachment.
		Body->AttachParent = GetMesh();
		Body->SetMasterPoseComponent(GetMesh());
	}

	Head = CreateOptionalDefaultSubobject< USkeletalMeshComponent>(TEXT("Head"));
	if (Head)
	{
		Head->AlwaysLoadOnClient = true;
		Head->AlwaysLoadOnServer = true;
		Head->bOwnerNoSee = false;
		Head->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
		Head->bCastDynamicShadow = true;
		Head->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Head->bChartDistanceFactor = true;
		Head->SetCollisionProfileName(CollisionProfileName);
		Head->bGenerateOverlapEvents = false;

		// Mesh acts as the head, as well as the parent for both animation and attachment.
		Head->AttachParent = GetMesh();
		Head->SetMasterPoseComponent(GetMesh());

	}
	Hand = CreateOptionalDefaultSubobject< USkeletalMeshComponent>(TEXT("Hand"));
	if (Hand)
	{
		Hand->AlwaysLoadOnClient = true;
		Hand->AlwaysLoadOnServer = true;
		Hand->bOwnerNoSee = false;
		Hand->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
		Hand->bCastDynamicShadow = true;
		Hand->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Hand->bChartDistanceFactor = true;
		Hand->SetCollisionProfileName(CollisionProfileName);
		Hand->bGenerateOverlapEvents = false;

		// Mesh acts as the head, as well as the parent for both animation and attachment.
		Hand->AttachParent = GetMesh();
		Hand->SetMasterPoseComponent(GetMesh());

	}
	Feet = CreateOptionalDefaultSubobject< USkeletalMeshComponent>(TEXT("Feet"));
	if (Feet)
	{
		Feet->AlwaysLoadOnClient = true;
		Feet->AlwaysLoadOnServer = true;
		Feet->bOwnerNoSee = false;
		Feet->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
		Feet->bCastDynamicShadow = true;
		Feet->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Feet->bChartDistanceFactor = true;
		Feet->SetCollisionProfileName(CollisionProfileName);
		Feet->bGenerateOverlapEvents = false;

		// Mesh acts as the head, as well as the parent for both animation and attachment.
		Feet->AttachParent = GetMesh();
		Feet->SetMasterPoseComponent(GetMesh());
	}

	Bodys.Add(Head);
	Bodys.Add(Body);
	Bodys.Add(Hand);
	Bodys.Add(Feet);

	//sockets.Add("WeaponDagger");
	//sockets.Add("WeaponAxe");
	//sockets.Add("WeaponSword");
	//sockets.Add("WeaponMace");
	//sockets.Add("WEAPON");
	//sockets.Add("AnimObjectR");
	//sockets.Add("SHIELD");
	//sockets.Add("AnimObjectL");
	//sockets.Add("WeaponBack");
	//sockets.Add("WeaponBow");
	//sockets.Add("QUIVER");

	equips.Init(NAME_None, 10);
	Weapons.AddDefaulted(2);

	State = ActionState::Idle;
	Target = NULL;

	race_type = TEXT("女人");
	class_type = TEXT("剑士");
	level = 1;
	attention_range = 5000.0f;
}

// Called when the game starts or when spawned
void AGameCharacter::BeginPlay()
{
	//记录默认的身体模型
	DefaultBodyMeshes.Reset();
	for (int i = 0; i < Bodys.Num(); i++)
	{
		DefaultBodyMeshes.Add(Bodys[i]->SkeletalMesh);
	}

	//调用蓝图初始化功能
	Super::BeginPlay();

	//绑定碰撞事件
	OnActorBeginOverlap.AddDynamic(this, &AGameCharacter::OnActorOverlap);

	//完全回复
	Recover();

	UpdateMesh();
	UpdateAnimGroup();
	UpdateClassSkills();
}

// Called every frame
void AGameCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AGameCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

int32 AGameCharacter::base_maxhp()
{
	Fconfig_class* data = UMyGameSingleton::Get().FindClass(class_type);
	check(data);
	int32 maxhp = data->hp + (level-1)*data->hp_add;

	for (int32 i = 0; i < equips.Num(); i++)
	{
		Fconfig_equip* equip_data = UMyGameSingleton::Get().FindEquip(equips[i]);
		if (equip_data != NULL)
		{
			maxhp += equip_data->hp_plus;
		}
	}
	
	return maxhp;
}
int32 AGameCharacter::base_maxmp()
{
	Fconfig_class* data = UMyGameSingleton::Get().FindClass(class_type);
	check(data);
	int32 maxmp = data->mp + (level - 1)*data->mp_add;

	for (int32 i = 0; i < equips.Num(); i++)
	{
		Fconfig_equip* equip_data = UMyGameSingleton::Get().FindEquip(equips[i]);
		if (equip_data != NULL)
		{
			maxmp += equip_data->mp_plus;
		}
	}

	return maxmp;
}

int32 AGameCharacter::base_patk()
{
	Fconfig_class* data = UMyGameSingleton::Get().FindClass(class_type);
	check(data);
	int32 rt = data->patk + (level - 1)*data->patk_add;

	for (int32 i = 0; i < equips.Num(); i++)
	{
		Fconfig_equip* equip_data = UMyGameSingleton::Get().FindEquip(equips[i]);
		if (equip_data != NULL)
		{
			rt += equip_data->patk_plus;
		}
	}

	return rt;
}

int32 AGameCharacter::base_matk()
{
	Fconfig_class* data = UMyGameSingleton::Get().FindClass(class_type);
	check(data);
	int32 rt = data->matk + (level - 1)*data->matk_add;

	for (int32 i = 0; i < equips.Num(); i++)
	{
		Fconfig_equip* equip_data = UMyGameSingleton::Get().FindEquip(equips[i]);
		if (equip_data != NULL)
		{
			rt += equip_data->matk_plus;
		}
	}

	return rt;
}


int32 AGameCharacter::base_pdef()
{
	Fconfig_class* data = UMyGameSingleton::Get().FindClass(class_type);
	check(data);
	int32 rt = data->pdef + (level - 1)*data->pdef_add;


	for (int32 i = 0; i < equips.Num(); i++)
	{
		Fconfig_equip* equip_data = UMyGameSingleton::Get().FindEquip(equips[i]);
		if (equip_data != NULL)
		{
			rt += equip_data->pdef_plus;
		}
	}

	return rt;
}

int32 AGameCharacter::base_mdef()
{
	Fconfig_class* data = UMyGameSingleton::Get().FindClass(class_type);
	check(data);
	int32 rt = data->mdef + (level - 1)*data->mdef_add;

	for (int32 i = 0; i < equips.Num(); i++)
	{
		Fconfig_equip* equip_data = UMyGameSingleton::Get().FindEquip(equips[i]);
		if (equip_data != NULL)
		{
			rt += equip_data->mdef_plus;
		}
	}

	return rt;
}
int32 AGameCharacter::base_hit()
{
	Fconfig_class* data = UMyGameSingleton::Get().FindClass(class_type);
	check(data);
	int32 rt = data->hit + (level - 1)*data->hit_add;

	for (int32 i = 0; i < equips.Num(); i++)
	{
		Fconfig_equip* equip_data = UMyGameSingleton::Get().FindEquip(equips[i]);
		if (equip_data != NULL)
		{
			rt += equip_data->hit_plus;
		}
	}

	return rt;
}
int32 AGameCharacter::base_eva()
{
	Fconfig_class* data = UMyGameSingleton::Get().FindClass(class_type);
	check(data);
	int32 rt = data->eva + (level - 1)*data->eva_add;

	for (int32 i = 0; i < equips.Num(); i++)
	{
		Fconfig_equip* equip_data = UMyGameSingleton::Get().FindEquip(equips[i]);
		if (equip_data != NULL)
		{
			rt += equip_data->eva_plus;
		}
	}

	return rt;
}


void AGameCharacter::Equip(FName id)
{
	Fconfig_equip* item = UMyGameSingleton::Get().FindEquip(id);
	if (item==NULL)
	{
		TRACE("无效的装备 %s",*id.ToString());
		return;
	}

	if (!ItemEnough(id,1))
		return;

		
	int32 slot = item->slot;

	ItemLose(id, 1);

	if (!equips[slot].IsNone())
		ItemAdd(equips[slot],1);

	equips[slot] = id;

	if (item->double_hand == 1)
	{
		if (slot == Fconfig_equip::MainHand && !equips[Fconfig_equip::SecondHand].IsNone())
		{
			ItemAdd(equips[Fconfig_equip::SecondHand], 1);
			equips[Fconfig_equip::SecondHand] = NAME_None;
		}
		else if (slot == Fconfig_equip::SecondHand && !equips[Fconfig_equip::MainHand].IsNone())
		{
			ItemAdd(equips[Fconfig_equip::MainHand], 1);
			equips[Fconfig_equip::MainHand] = NAME_None;
		}
	}
	
	UpdateMesh();

	UpdateAnimGroup();
}

void AGameCharacter::UnEquip(int32 slot)
{
	if (!equips[slot].IsNone())
	{
		ItemAdd(equips[slot], 1);
		equips[slot] = NAME_None;

		UpdateMesh();
		UpdateAnimGroup();
	}
}

void AGameCharacter::UpdateMesh()
{
	
	Fconfig_race* race_data = UMyGameSingleton::Get().FindRace(race());
	for (int i = 0; i < equips.Num(); i++)
	{
		Fconfig_equip* equip = UMyGameSingleton::Get().FindEquip(equips[i]);
		if (equip != NULL)
		{
			switch (equip->slot)
			{
#if USE_BODY_EQUIP
			case Fconfig_equip::Head:
			case Fconfig_equip::Body:
			case Fconfig_equip::Hand:
			case Fconfig_equip::Feet:
			{
				Fconfig_armor_map* armor = UMyGameSingleton::Get().FindArmorMap(equips[i], race);
				if (armor != NULL)
					Bodys[equip->slot]->SetSkeletalMesh(Cast<USkeletalMesh>(armor->model.ToStringReference().TryLoad()));
				else
					TRACE("armor == NULL %s", *equips[i].ToString());
				break;
			}
#endif
			case Fconfig_equip::MainHand:
			case Fconfig_equip::SecondHand:
			{
				int weapon_index = equip->slot - Fconfig_equip::MainHand;
				if (Weapons[weapon_index]!=NULL)
				{
					if (Weapons[weapon_index]->GetID() == equips[i])
						break;

					Weapons[weapon_index]->OnUnEquip();
					Weapons[weapon_index] = NULL;

				}
				if (!equips[i].IsNone())
				{
					Weapons[weapon_index] = UWeaponBase::Create(this, equip->weapon_type);
					Weapons[weapon_index]->OnEquip(equips[i]);
				}
			}
				break;
			default:
				break;
			}
		}
		else
		{
			switch (i)
			{
#if USE_BODY_EQUIP
			case Fconfig_equip::Head:
			{
				GetMesh()->SetSkeletalMesh(DefaultBodyMeshes[i]);
				Bodys[i]->SetSkeletalMesh(NULL);
			}
			break;
			case Fconfig_equip::Body:
			case Fconfig_equip::Hand:
			case Fconfig_equip::Feet:
				Bodys[i]->SetSkeletalMesh(DefaultBodyMeshes[i]);
				break;
#endif
			case Fconfig_equip::MainHand:
			case Fconfig_equip::SecondHand:
			{
				int weapon_index = i - Fconfig_equip::MainHand;
				if (Weapons[weapon_index] != NULL && Weapons[weapon_index]->GetID() != equips[i])
				{
					Weapons[weapon_index]->OnUnEquip();
					Weapons[weapon_index] = NULL;
				}
			}
				break;
			default:
				break;
			}
		}
	}
}


void AGameCharacter::ChangeWeaponState()
{
	if (IsWeaponOpen())
	{
		for (int32 i = 0; i < Weapons.Num(); i++)
		{
			if (Weapons[i]!=NULL)
				Weapons[i]->Close();
		}
	}
	else
	{
		for (int32 i = 0; i < Weapons.Num(); i++)
		{
			if (Weapons[i] != NULL)
				Weapons[i]->Open();
		}
	}

	UpdateAnimGroup();
}

bool AGameCharacter::IsWeaponOpen()
{
	if(Weapons[0]!=NULL && Weapons[0]->IsOpen())
		return true;
	return false;
}


void AGameCharacter::UpdateAnimGroup()
{
	Fconfig_race* race_data = UMyGameSingleton::Get().FindRace(race());
	anim_group = race_data->anim_group;
	anim_openweapon = NULL;
	anim_closeweapon = NULL;
	anim_block = NULL;
	int block_priorit = 0;
	if (Weapons[0] != NULL)
	{
		Fconfig_weapon_map* weaponMap = UMyGameSingleton::Get().FindWeaponMap(Weapons[0]->GetID(), race());

		weaponMap->open_weapon.LoadSynchronous();
		weaponMap->close_weapon.LoadSynchronous();
		anim_block = weaponMap->block.LoadSynchronous();
		block_priorit = weaponMap->block_priorit;

		anim_openweapon = weaponMap->open_weapon;
		anim_closeweapon = weaponMap->close_weapon;
			
		if (Weapons[0]->IsOpen())
		{
			anim_group = weaponMap->anim_group;
		}
	}
	if (Weapons[1] != NULL)
	{
		Fconfig_weapon_map* weaponMap = UMyGameSingleton::Get().FindWeaponMap(Weapons[0]->GetID(), race());
		if (weaponMap->block_priorit > block_priorit)
		{
			anim_block = weaponMap->block.LoadSynchronous();
		}
	}

	Fconfig_anim_group* ag = UMyGameSingleton::Get().FindAnimGroup(anim_group);
	if (ag != NULL)
	{
		if (ag->movement.Get() == NULL)
			ag->movement.ToStringReference().TryLoad();
		if (ag->block_bash.Get() == NULL)
			ag->block_bash.ToStringReference().TryLoad();
		if (ag->block_hit.Get() == NULL)
			ag->block_hit.ToStringReference().TryLoad();
		if (ag->block_idle.Get() == NULL)
			ag->block_idle.ToStringReference().TryLoad();
		if (ag->jump_fall.Get() == NULL)
			ag->jump_fall.ToStringReference().TryLoad();
		if (ag->jump_land.Get() == NULL)
			ag->jump_land.ToStringReference().TryLoad();
		if (ag->jump_start.Get() == NULL)
			ag->jump_start.ToStringReference().TryLoad();

	}
	else
	{
		TRACE("Invalid Anim Group %s", *anim_group.ToString());
	}
}

int32 AGameCharacter::ItemAdd(FName id, int32 count)
{
	if (!items.Contains(id))
		items.Add(id);

	items[id] += count;
	items[id] = FMath::Clamp(items[id], 0, ItemMax(id));
	return items[id];
}

int32 AGameCharacter::ItemLose(FName id, int32 count)
{
	if (!items.Contains(id))
		items.Add(id);
	items[id] -= count;
	items[id] = FMath::Clamp(items[id], 0, ItemMax(id));
	return items[id];
}


int32 AGameCharacter::ItemMax(FName id)
{
	Fconfig_item* itemData = UMyGameSingleton::Get().FindItem(id);
	if (itemData != NULL)
		return itemData->max;
	return 0;
}

bool AGameCharacter::ItemEnough(FName id, int32 count)
{
	if (!items.Contains(id))
		items.Add(id);
	return items[id] >= count;
}

TArray<FItem> AGameCharacter::GetItems()
{
	TArray<FItem> data;
	for (auto kv : items)
	{
		if (kv.Value > 0)
		{
			FItem item;
			item.Name = kv.Key;
			item.Num = kv.Value;
			data.Add(item);
		}
	}
	return data;
}

TArray<UTask*> AGameCharacter::GetTasks()
{
	TArray<UTask*> data;
	for (auto kv : tasks)
	{
		data.Add(kv.Value);
	}
	return data;
}

void AGameCharacter::OnActorOverlap(AActor* OtherActor)
{
	if (OtherActor->GetAttachParentActor() != this)
		TRACE("OnActorOverlap %s", *OtherActor->GetName());
}




void AGameCharacter::AnimNofity_TakeArrow()
{
	if (Weapons[0]!=NULL)
		Weapons[0]->AttackStart();
}

void AGameCharacter::AnimNofity_Shoot()
{
	Super::AnimNofity_Shoot();

	if (current_skill == NULL)
		return;
	if (Target == NULL)
		return;

	Fconfig_effect* effect = UMyGameSingleton::Get().FindEffect(current_skill->id, race());
	if (effect != NULL)
	{
		Weapons[0]->AttackEnd();
	}
}

bool AGameCharacter::Attack(FName skillId)
{
	if (Target == NULL)
	{
		if (!skills.Contains(skillId))
			return false;

		USkill* skill = skills[skillId];

		if (!AutoSelectTarget(skill))
			return false;
	}

	if (!Super::Attack(skillId))
		return false;

	//播放武器动画
	Fconfig_effect* effect = UMyGameSingleton::Get().FindEffect(current_skill->id, race());
	if (effect != NULL && effect->start_weapon_anim!=NULL)
	{
		for (auto weapon : Weapons)
		{
			if (weapon != NULL)
				weapon->PlayAnim(effect->start_weapon_anim);
		}
	}
	return true;
}
void AGameCharacter::SkillEffect(AGameBattler* User, USkill* skill)
{
	Super::SkillEffect(User, skill);

	if (hp == 0)
	{
		this->State = ActionState::Dead;
	}
}


bool AGameCharacter::can_move()
{
	if (!Super::can_move())
		return false;
	return State == ActionState::Idle || State == ActionState::Move;
}

bool AGameCharacter::can_use_skill(USkill* skill)
{
	if (!Super::can_use_skill(skill))
		return false;
	if (!IsWeaponOpen())
		return false;
	return true;
}

bool AGameCharacter::can_block()
{
	if (!(State == ActionState::Idle || State == ActionState::Move))
	{
		return false;
	}
	if (!IsWeaponOpen())
		return false;
	return true;
}


void AGameCharacter::TaskAdd(FName id)
{
	Fconfig_task* data = UMyGameSingleton::Get().FindTask(id);
	if (data == NULL)
	{
		UE_LOG(client, Warning, TEXT("Task Not Found %s"), *id.ToString());
		return;
	}
	if (tasks.Contains(id))
	{
		UE_LOG(client, Warning, TEXT("Task Has Bean Added %s"), *id.ToString());
		return;
	}

	UTask* t = NULL;
	if (data->prefab != NULL)
	{
		t = NewObject<UTask>(this, data->prefab);
	}
	else
	{
		t = NewObject<UTask>();
	}
	t->id = id;
	tasks.Add(id, t);
}

void AGameCharacter::TaskFinish(FName id)
{
	if (!tasks.Contains(id))
	{
		UE_LOG(client, Warning, TEXT("Task  %s Not Found"), *id.ToString());
		return;
	}
	UTask* t = tasks[id];
	if (t->State == UTask::TaskState::GOING)
		t->State = UTask::TaskState::FINISHED;
}

void AGameCharacter::TaskReward(FName id)
{
	if (!tasks.Contains(id))
	{
		UE_LOG(client, Warning, TEXT("Task  %s Not Found"), *id.ToString());
		return;
	}
	UTask* t = tasks[id];
	if (t->State != UTask::TaskState::FINISHED)
		return;
	for (auto itemData : t->GetData()->rewards)
	{
		this->ItemAdd(itemData.Name, itemData.Num);
	}
}


FString AGameCharacter::GetAttributeText()
{
	return FString::Printf(TEXT("生命值:%d/%d        魔法值%d/%d\n\
		物攻:%d        魔攻:%d\n\
		物防:%d        魔防:%d\n\
		命中:%d%%      闪避:%d%%"
		), hp,maxhp(),mp,maxmp(),patk(),matk(),pdef(),mdef(),(int32)(hit()/100.0f),(int32)(eva()/100.0f));
}

void AGameCharacter::SerializeProperty(FArchive& ar)
{
	ar << hp << mp << (uint8&)(camp) << skills << level << exp << class_type << race_type << equips << items << tasks;
}

#define MAX_LEVEL 80
void AGameCharacter::AddExp(int32 e)
{
	int32 max_exp = UMyGameSingleton::Get().GetLevelExp(level);
	if (level >= MAX_LEVEL)
	{
		exp = max_exp;
		return;
	}
	exp += e;
	while (exp >= max_exp)
	{
		exp -= max_exp;
		LevelUp();
		max_exp = UMyGameSingleton::Get().GetLevelExp(level);
	}
}


void AGameCharacter::LevelUp()
{
	if (level < MAX_LEVEL)
	{
		level++;

		UpdateClassSkills();

		//播放升级音效和特效
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), UMyGameSingleton::Get().sound_levelup, GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(Target->GetWorld(), UMyGameSingleton::Get().ps_levelup, GetActorLocation());
	}
}

void AGameCharacter::UpdateClassSkills()
{
	//学习新技能
	Fconfig_class* classData = UMyGameSingleton::Get().FindClass(class_type);
	for (auto skill : classData->skills)
	{
		if (level >= skill.level && !skills.Contains(skill.Name))
		{
			LearnSkill(skill.Name);
		}
	}
}


bool AGameCharacter::AutoSelectTarget(USkill* skill)
{
	TArray<AGameBattler*> targets = this->FindBattlers(this->attention_range);
	if (this->Target != NULL || skill == NULL)
		targets.Remove(this->Target);

	if (skill == NULL && targets.Num()>0)
	{
		this->SelectTarget(targets[0]);
		return true;
	}

	for (auto bt : targets)
	{
		if (skill->valid_target(this, bt))
		{
			this->SelectTarget(bt);
			return true;
		}
	}

	return false;
}
