// Fill out your copyright notice in the Description page of Project Settings.

#include "client.h"
#include "MyCharacter.h"
#include "MyGameSingleton.h"
#include "config.h"
#include "Weapon.h"

const FString ContextString = "MyCharacter";

DEFINE_LOG_CATEGORY(client);

#define WEAPON_SLOT_START  4

// Sets default values
AMyCharacter::AMyCharacter()
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
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
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
	OnActorBeginOverlap.AddDynamic(this, &AMyCharacter::OnActorOverlap);

	//完全回复
	Recover();
}

// Called every frame
void AMyCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

int32 AMyCharacter::base_maxhp()
{
	int32 maxhp = 100 + level * 50;

	for (int32 i = 0; i < equips.Num(); i++)
	{
		Fconfig_equip* data = UMyGameSingleton::Get().FindEquip(equips[i]);
		if (data != NULL)
		{
			maxhp += data->hp_plus;
		}
	}
	
	return maxhp;
}
int32 AMyCharacter::base_maxmp()
{

	int32 maxmp = 100 + level * 50;

	for (int32 i = 0; i < equips.Num(); i++)
	{
		Fconfig_equip* data = UMyGameSingleton::Get().FindEquip(equips[i]);
		if (data != NULL)
		{
			maxmp += data->mp_plus;
		}
	}

	return maxmp;
}

int32 AMyCharacter::base_patk()
{
	int32 rt = level * 20;

	for (int32 i = 0; i < equips.Num(); i++)
	{
		Fconfig_equip* data = UMyGameSingleton::Get().FindEquip(equips[i]);
		if (data != NULL)
		{
			rt += data->patk_plus;
		}
	}

	return rt;
}

int32 AMyCharacter::base_matk()
{
	int32 rt = level * 20;

	for (int32 i = 0; i < equips.Num(); i++)
	{
		Fconfig_equip* data = UMyGameSingleton::Get().FindEquip(equips[i]);
		if (data != NULL)
		{
			rt += data->matk_plus;
		}
	}

	return rt;
}


int32 AMyCharacter::base_pdef()
{
	int32 rt = level * 5;

	for (int32 i = 0; i < equips.Num(); i++)
	{
		Fconfig_equip* data = UMyGameSingleton::Get().FindEquip(equips[i]);
		if (data != NULL)
		{
			rt += data->pdef_plus;
		}
	}

	return rt;
}

int32 AMyCharacter::base_mdef()
{
	int32 rt = level * 5;

	for (int32 i = 0; i < equips.Num(); i++)
	{
		Fconfig_equip* data = UMyGameSingleton::Get().FindEquip(equips[i]);
		if (data != NULL)
		{
			rt += data->mdef_plus;
		}
	}

	return rt;
}


void AMyCharacter::Equip(FName id)
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

void AMyCharacter::UnEquip(int32 slot)
{
	if (!equips[slot].IsNone())
	{
		ItemAdd(equips[slot], 1);
		equips[slot] = NAME_None;

		UpdateMesh();
		UpdateAnimGroup();
	}
}

void AMyCharacter::UpdateMesh()
{
	
	Fconfig_race* race_data = UMyGameSingleton::Get().FindRace(race);
	for (int i = 0; i < equips.Num(); i++)
	{
		Fconfig_equip* equip = UMyGameSingleton::Get().FindEquip(equips[i]);
		if (equip != NULL)
		{
			switch (equip->slot)
			{
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


void AMyCharacter::ChangeWeaponState()
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

bool AMyCharacter::IsWeaponOpen()
{
	if(Weapons[0]!=NULL && Weapons[0]->IsOpen())
		return true;
	return false;
}


void AMyCharacter::UpdateAnimGroup()
{
	Fconfig_race* race_data = UMyGameSingleton::Get().FindRace(race);
	anim_group = race_data->anim_group;
	anim_openweapon = NULL;
	anim_closeweapon = NULL;

	if (Weapons[0] != NULL)
	{
		Fconfig_weapon_map* weaponMap = UMyGameSingleton::Get().FindWeaponMap(Weapons[0]->GetID(), race);

		if (weaponMap->open_weapon.Get() == NULL)
			weaponMap->open_weapon.ToStringReference().TryLoad();
		if (weaponMap->close_weapon.Get() == NULL)
			weaponMap->close_weapon.ToStringReference().TryLoad();

		anim_openweapon = weaponMap->open_weapon;
		anim_closeweapon = weaponMap->close_weapon;
			
		if (Weapons[0]->IsOpen())
		{
			anim_group = weaponMap->anim_group;
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

int32 AMyCharacter::ItemAdd(FName id, int32 count)
{
	if (!items.Contains(id))
		items.Add(id);

	items[id] += count;
	items[id] = FMath::Clamp(items[id], 0, ItemMax(id));
	return items[id];
}

int32 AMyCharacter::ItemLose(FName id, int32 count)
{
	if (!items.Contains(id))
		items.Add(id);
	items[id] -= count;
	items[id] = FMath::Clamp(items[id], 0, ItemMax(id));
	return items[id];
}


int32 AMyCharacter::ItemMax(FName id)
{
	Fconfig_item* itemData = UMyGameSingleton::Get().FindItem(id);
	if (itemData != NULL)
		return itemData->max;
	return 0;
}

bool AMyCharacter::ItemEnough(FName id, int32 count)
{
	if (!items.Contains(id))
		items.Add(id);
	return items[id] >= count;
}


void AMyCharacter::OnActorOverlap(AActor* OtherActor)
{
	if (OtherActor->GetAttachParentActor() != this)
		TRACE("OnActorOverlap %s", *OtherActor->GetName());
}




void AMyCharacter::AnimNofity_TakeArrow()
{
	if (Weapons[0]!=NULL)
		Weapons[0]->AttackStart();
}

void AMyCharacter::AnimNofity_Shoot()
{
	Super::AnimNofity_Shoot();

	if (current_skill == NULL)
		return;
	if (Target == NULL)
		return;

	Fconfig_effect* effect = UMyGameSingleton::Get().FindEffect(current_skill->id, race);
	if (effect != NULL)
	{
		Weapons[0]->AttackEnd();
	}
}

void AMyCharacter::SkillEffect(AGameBattler* User, USkill* skill)
{
	Super::SkillEffect(User, skill);

	if (hp == 0)
	{
		this->State = ActionState::Dead;
	}
}


bool AMyCharacter::can_move()
{
	if (!Super::can_move())
		return false;
	return State == ActionState::Idle || State == ActionState::Move;
}

bool AMyCharacter::can_use_skill()
{
	if (!Super::can_use_skill())
		return false;
	if (!IsWeaponOpen())
		return false;
	return true;
}