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
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	

	for (int i = 0; i < Bodys.Num(); i++)
	{
		DefaultBodyMeshes[i] = Bodys[i]->SkeletalMesh;
	}
	DefaultAnimGroup = GetMesh()->GetAnimInstance()->GetFName();

	UE_LOG(client, Log, TEXT("Begin Play"));

	Super::BeginPlay();
	//最多6件装备
	//equips.Init(0, 6);
	//items.Init(0, UMyGameSingleton::Get().config_item->RowMap.Num());
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

void AMyCharacter::Reset()
{
	hp = maxhp();
}

int32 AMyCharacter::maxhp()
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

int32 AMyCharacter::patk()
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

int32 AMyCharacter::matk()
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


int32 AMyCharacter::pdef()
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

int32 AMyCharacter::mdef()
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


void AMyCharacter::Equip(int32 slot,FName id)
{
	if (!id.IsNone())
	{
		if (!ItemEnough(id,1))
			return;

		Fconfig_equip* item = UMyGameSingleton::Get().FindEquip(id);

		if (item->slot != slot)
		{
			TRACE("equip error slot %d,item %s", slot, *id.ToString());
			return;
		}

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
	}
	else
	{
		if (!equips[slot].IsNone())
		{
			ItemAdd(equips[slot], 1);
			equips[slot] = NAME_None;
		}
	}

	
	
	//weapon_state = !main_weapon().IsNone();

	UpdateMesh();

	UpdateAnimGroup();
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
				if (Weapons[weapon_index]!=NULL && Weapons[weapon_index]->GetID() != equips[i])
				{
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
			//{

			//	if (mh_weapon != NULL)
			//	{
			//		mh_weapon->Destroy();
			//		mh_weapon = NULL;
			//	}
			//	if (mh_append != NULL)
			//	{
			//		mh_append->Destroy();
			//		mh_append = NULL;
			//	}

			//	if (equip->double_hand)
			//	{
			//		if (sh_weapon != NULL)
			//		{
			//			sh_weapon->Destroy();
			//			sh_weapon = NULL;
			//		}
			//		if (sh_append != NULL)
			//		{
			//			sh_append->Destroy();
			//			sh_append = NULL;
			//		}
			//	}

			//	Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(equips[i], race);
			//	if (weapon != NULL)
			//	{
			//		
			//		UStaticMesh* staticMesh = Cast<UStaticMesh>(weapon->model.ToStringReference().TryLoad());
			//		USkeletalMesh* skeletalMesh = Cast<USkeletalMesh>(weapon->model.ToStringReference().TryLoad());

			//		if (staticMesh != NULL)
			//		{
			//			AStaticMeshActor* actor_weapon = Cast<AStaticMeshActor>( GetWorld()->SpawnActor(*templateSword));
			//			actor_weapon->GetStaticMeshComponent()->SetStaticMesh(staticMesh);
			//			mh_weapon = actor_weapon;
			//		}
			//		else if (skeletalMesh != NULL)
			//		{
			//			ASkeletalMeshActor* actor_weapon = Cast<ASkeletalMeshActor>(GetWorld()->SpawnActor(*templateBow));
			//			actor_weapon->GetSkeletalMeshComponent()->SetSkeletalMesh(skeletalMesh);
			//			mh_weapon = actor_weapon;
			//		}

			//		const USkeletalMeshSocket* sc = GetMesh()->GetSocketByName(*weapon->slot);
			//		if (sc != NULL)
			//			sc->AttachActor(mh_weapon, GetMesh());
			//		else
			//			TRACE("sc == NULL  %s ", *weapon->slot);

			//		//附件
			//		staticMesh = Cast<UStaticMesh>(weapon->append_1.ToStringReference().TryLoad());
			//		if (mh_append == NULL)
			//		{
			//			mh_append = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
			//			mh_append->SetMobility(EComponentMobility::Movable);
			//		}
			//		mh_append->GetStaticMeshComponent()->SetStaticMesh(staticMesh);

			//		sc = GetMesh()->GetSocketByName(*weapon->slot1);
			//		if (sc != NULL)
			//			sc->AttachActor(mh_append, GetMesh());
			//		else
			//			TRACE("sc == NULL  %s ", *weapon->slot1);
			//	}
			//}
			//	break;
			//case Fconfig_equip::SecondHand:
			//{
			//	if (sh_weapon != NULL)
			//	{
			//		sh_weapon->Destroy();
			//		sh_weapon = NULL;
			//	}
			//	if (sh_append != NULL)
			//	{
			//		sh_append->Destroy();
			//		sh_append = NULL;
			//	}
			//	if (equip->double_hand)
			//	{
			//		if (mh_weapon != NULL)
			//		{
			//			mh_weapon->Destroy();
			//			mh_weapon = NULL;
			//		}
			//		if (mh_append != NULL)
			//		{
			//			mh_append->Destroy();
			//			mh_append = NULL;
			//		}
			//	}

			//	Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(equips[i], race);
			//	if (weapon != NULL)
			//	{
			//		UStaticMesh* staticMesh = Cast<UStaticMesh>(weapon->model.ToStringReference().TryLoad());
			//		USkeletalMesh* skeletalMesh = Cast<USkeletalMesh>(weapon->model.ToStringReference().TryLoad());

			//		if (staticMesh != NULL)
			//		{
			//			AStaticMeshActor* actor_weapon = Cast<AStaticMeshActor>(GetWorld()->SpawnActor(*templateSword));
			//			actor_weapon->GetStaticMeshComponent()->SetStaticMesh(staticMesh);
			//			sh_weapon = actor_weapon;
			//		}
			//		else if (skeletalMesh != NULL)
			//		{
			//			ASkeletalMeshActor* actor_weapon = Cast<ASkeletalMeshActor>(GetWorld()->SpawnActor(*templateBow));
			//			actor_weapon->GetSkeletalMeshComponent()->SetSkeletalMesh(skeletalMesh);
			//			sh_weapon = actor_weapon;
			//		}

			//		const USkeletalMeshSocket* sc = GetMesh()->GetSocketByName(*weapon->slot);
			//		if (sc != NULL)
			//			sc->AttachActor(sh_weapon, GetMesh());
			//		else
			//			TRACE("sc == NULL  %s ", *weapon->slot);

			//		//附件
			//		staticMesh = Cast<UStaticMesh>(weapon->append_1.ToStringReference().TryLoad());
			//		if (sh_append == NULL)
			//		{
			//			sh_append = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
			//			sh_append->SetMobility(EComponentMobility::Movable);
			//		}
			//		sh_append->GetStaticMeshComponent()->SetStaticMesh(staticMesh);

			//		sc = GetMesh()->GetSocketByName(*weapon->slot1);
			//		if (sc != NULL)
			//			sc->AttachActor(sh_append, GetMesh());
			//		else
			//			TRACE("sc == NULL  %s ", *weapon->slot1);

			//	}
			//}
			//	break;
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

//
//void AMyCharacter::OpenWeapon()
//{
//	/*if (weapon_state == 1)
//		return;
//	weapon_state = 1;
//	
//	{
//		Fconfig_equip* equip = UMyGameSingleton::Get().FindEquip(equips[Fconfig_equip::MainHand]);
//		Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(equips[Fconfig_equip::MainHand], race);
//		if (weapon != NULL)
//		{
//			if (mh_weapon != NULL)
//			{
//				mh_weapon->DetachRootComponentFromParent();
//				const USkeletalMeshSocket* sc = GetMesh()->GetSocketByName(*weapon->slot);
//				if (sc != NULL)
//					sc->AttachActor(mh_weapon, GetMesh());
//				else
//					TRACE("sc == NULL  %s ", *weapon->slot);
//
//			}
//		}
//	}
//	{
//		Fconfig_equip* equip = UMyGameSingleton::Get().FindEquip(equips[Fconfig_equip::SecondHand]);
//		Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(equips[Fconfig_equip::SecondHand], race);
//		if (weapon != NULL)
//		{
//			if (sh_weapon != NULL)
//			{
//				sh_weapon->DetachRootComponentFromParent();
//				const USkeletalMeshSocket* sc = GetMesh()->GetSocketByName(*weapon->slot);
//				if (sc != NULL)
//					sc->AttachActor(sh_weapon, GetMesh());
//				else
//					TRACE("sc == NULL  %s ", *weapon->slot);
//
//			}
//		}
//	}
//
//	UpdateAnimGroup();*/
//}
//
//void AMyCharacter::CloseWeapon()
//{
//	if (weapon_state == 0)
//		return;
//	weapon_state = 0;
//
//	{
//		Fconfig_equip* equip = UMyGameSingleton::Get().FindEquip(equips[Fconfig_equip::MainHand]);
//		Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(equips[Fconfig_equip::MainHand], race);
//		if (weapon != NULL)
//		{
//			if (mh_weapon != NULL)
//			{
//				mh_weapon->DetachRootComponentFromParent();
//				const USkeletalMeshSocket* sc = GetMesh()->GetSocketByName(*weapon->slot1);
//				if (sc != NULL)
//					sc->AttachActor(mh_weapon, GetMesh());
//				else
//					TRACE("sc == NULL  %s ", *weapon->slot1);
//
//			}
//		}
//	}
//	{
//		Fconfig_equip* equip = UMyGameSingleton::Get().FindEquip(equips[Fconfig_equip::SecondHand]);
//		Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(equips[Fconfig_equip::SecondHand], race);
//		if (weapon != NULL)
//		{
//			if (sh_weapon != NULL)
//			{
//				sh_weapon->DetachRootComponentFromParent();
//				const USkeletalMeshSocket* sc = GetMesh()->GetSocketByName(*weapon->slot1);
//				if (sc != NULL)
//					sc->AttachActor(sh_weapon, GetMesh());
//				else
//					TRACE("sc == NULL  %s ", *weapon->slot1);
//
//			}
//		}
//	}
//
//	UpdateAnimGroup();
//}
//

void AMyCharacter::UpdateAnimGroup()
{
	Fconfig_race* race_data = UMyGameSingleton::Get().FindRace(race);
	anim_group = DefaultAnimGroup;

	for (int i = 0; i < Weapons.Num(); i++)
	{
		if (Weapons[i] != NULL && Weapons[i]->IsOpen())
		{
			Fconfig_weapon_map* weaponMap = UMyGameSingleton::Get().FindWeaponMap(Weapons[i]->GetID(), race);
			anim_group = weaponMap->anim_group;

			if (weaponMap->open_weapon.Get() == NULL)
				weaponMap->open_weapon.ToStringReference().TryLoad();
			if (weaponMap->close_weapon.Get() == NULL)
				weaponMap->close_weapon.ToStringReference().TryLoad();
			break;
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

	//if (weapon_state == 0)
	//{
	//	Fconfig_race* race_data = UMyGameSingleton::Get().FindRace(race);
	//	anim_group = race_data->anim_group;
	//}
	//else
	//{
	//	if (!main_weapon().IsNone())
	//	{
	//		Fconfig_weapon_map* weaponMap = UMyGameSingleton::Get().FindWeaponMap(main_weapon(), race);
	//		anim_group = weaponMap->anim_group;

	//		if (weaponMap->open_weapon.Get() == NULL)
	//			weaponMap->open_weapon.ToStringReference().TryLoad();
	//		if (weaponMap->close_weapon.Get() == NULL)
	//			weaponMap->close_weapon.ToStringReference().TryLoad();
	//	}
	//	else
	//	{
	//		Fconfig_race* race_data = UMyGameSingleton::Get().FindRace(race);
	//		anim_group = race_data->anim_group;
	//	}
	//}
	//
	////TRACE("ag %s ,weaponstate %d", *anim_group.ToString(), weapon_state);

	//Fconfig_anim_group* ag = UMyGameSingleton::Get().FindAnimGroup(anim_group);
	//if (ag != NULL)
	//{
	//	if (ag->movement.Get() == NULL)
	//		ag->movement.ToStringReference().TryLoad();
	//	if (ag->block_bash.Get() == NULL)
	//		ag->block_bash.ToStringReference().TryLoad();
	//	if (ag->block_hit.Get() == NULL)
	//		ag->block_hit.ToStringReference().TryLoad();
	//	if (ag->block_idle.Get() == NULL)
	//		ag->block_idle.ToStringReference().TryLoad();
	//	if (ag->jump_fall.Get() == NULL)
	//		ag->jump_fall.ToStringReference().TryLoad();
	//	if (ag->jump_land.Get() == NULL)
	//		ag->jump_land.ToStringReference().TryLoad();
	//	if (ag->jump_start.Get() == NULL)
	//		ag->jump_start.ToStringReference().TryLoad();

	//}
	//else
	//{
	//	TRACE("Invalid Anim Group %s", *anim_group.ToString());
	//}


	//anim_openweapon = Anim_OpenWeapon();
	//anim_closeweapon = Anim_CloseWeapon();
}
//
//FName AMyCharacter::main_weapon()
//{
//	if (!equips[Fconfig_equip::MainHand].IsNone())
//	return equips[Fconfig_equip::MainHand];
//
//	return equips[Fconfig_equip::SecondHand];
//}
//
//TAssetPtr<UAnimMontage> AMyCharacter::Anim_OpenWeapon()
//{
//	FName mw = main_weapon();
//	if (!mw.IsNone())
//	{
//		Fconfig_weapon_map* weaponMap = UMyGameSingleton::Get().FindWeaponMap(mw, race);
//		//anim_group = weaponMap->anim_group;
//
//		if (weaponMap->open_weapon.Get() == NULL)
//			weaponMap->open_weapon.ToStringReference().TryLoad();
//		return weaponMap->open_weapon;
//	}
//	return NULL;
//}
//TAssetPtr<UAnimMontage> AMyCharacter::Anim_CloseWeapon()
//{
//	FName mw = main_weapon();
//	if (!mw.IsNone())
//	{
//		Fconfig_weapon_map* weaponMap = UMyGameSingleton::Get().FindWeaponMap(mw, race);
//		//anim_group = weaponMap->anim_group;
//
//		if (weaponMap->close_weapon.Get() == NULL)
//			weaponMap->close_weapon.ToStringReference().TryLoad();
//		return weaponMap->close_weapon;
//	}
//	return NULL;
//}


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