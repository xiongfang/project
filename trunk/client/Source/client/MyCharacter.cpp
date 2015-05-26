// Fill out your copyright notice in the Description page of Project Settings.

#include "client.h"
#include "MyCharacter.h"
#include "MyGameSingleton.h"
#include "config.h"


const FString ContextString = "MyCharacter";

DEFINE_LOG_CATEGORY(client);

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	static FName CollisionProfileName(TEXT("IgnoreOnlyPawn"));
	Head = CreateDefaultSubobject< USkeletalMeshComponent>(TEXT("HeadComponent"));
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

		//Components.Add(Body);
	}

	Body = CreateDefaultSubobject< USkeletalMeshComponent>(TEXT("BodyComponent"));
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

	Hand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandComponent"));
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

	Feet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FeetComponent"));
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

	mh_weapon = NULL;
	mh_append = NULL;
	sh_weapon = NULL;
	sh_append = NULL;

	weapon_state = 0;

	DefaultAnimBP = NULL;
	DefaultHead = NULL;
	DefaultBody = NULL;
	DefaultHand = NULL;
	DefaultFeet = NULL;
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	DefaultAnimBP = GetMesh()->GetAnimInstance()->GetClass();
	DefaultHead = GetMesh()->SkeletalMesh;
	DefaultBody = Body->SkeletalMesh;
	DefaultHand = Hand->SkeletalMesh;
	DefaultFeet = Feet->SkeletalMesh;

}
// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	
	Super::BeginPlay();
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

		if (slot == Fconfig_equip::MainHand && item->double_hand == 1)
		{
			if (!equips[Fconfig_equip::SecondHand].IsNone())
			{
				ItemAdd(equips[Fconfig_equip::SecondHand], 1);
				equips[Fconfig_equip::SecondHand] = NAME_None;
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

	

	UpdateMesh();
	UpdateAnimBP();
}

FName AMyCharacter::main_weapon_map()
{
	return UMyGameSingleton::MakeKey(equips[Fconfig_equip::MainHand], race);
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
			{
				Fconfig_armor_map* armor = UMyGameSingleton::Get().FindArmorMap(equips[i], race);
				Head->SetSkeletalMesh(Cast<USkeletalMesh>(armor->model.ToStringReference().TryLoad()));
				break;
			}
			case Fconfig_equip::Body:
			{
				Fconfig_armor_map* armor = UMyGameSingleton::Get().FindArmorMap(equips[i], race);
				Body->SetSkeletalMesh(Cast<USkeletalMesh>(armor->model.ToStringReference().TryLoad()));
				break;
			}
			case Fconfig_equip::Hand:
			{
				Fconfig_armor_map* armor = UMyGameSingleton::Get().FindArmorMap(equips[i], race);
				Hand->SetSkeletalMesh(Cast<USkeletalMesh>(armor->model.ToStringReference().TryLoad()));
				break;
			}
			case Fconfig_equip::Feet:
			{
				Fconfig_armor_map* armor = UMyGameSingleton::Get().FindArmorMap(equips[i], race);
				Feet->SetSkeletalMesh(Cast<USkeletalMesh>(armor->model.ToStringReference().TryLoad()));
				break;
			}
			case Fconfig_equip::MainHand:
			{

				if (mh_weapon != NULL)
				{
					mh_weapon->Destroy();
					mh_weapon = NULL;
				}
				if (mh_append != NULL)
				{
					mh_append->Destroy();
					mh_append = NULL;
				}

				if (equip->double_hand)
				{
					if (sh_weapon != NULL)
					{
						sh_weapon->Destroy();
						sh_weapon = NULL;
					}
					if (sh_append != NULL)
					{
						sh_append->Destroy();
						sh_append = NULL;
					}
				}

				Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(equips[i], race);
				if (weapon != NULL)
				{
					
					UStaticMesh* staticMesh = Cast<UStaticMesh>(weapon->model.ToStringReference().TryLoad());
					USkeletalMesh* skeletalMesh = Cast<USkeletalMesh>(weapon->model.ToStringReference().TryLoad());

					if (staticMesh != NULL)
					{
						AStaticMeshActor* actor_weapon = Cast<AStaticMeshActor>( GetWorld()->SpawnActor(*templateSword));
						actor_weapon->GetStaticMeshComponent()->SetStaticMesh(staticMesh);
						mh_weapon = actor_weapon;
					}
					else if (skeletalMesh != NULL)
					{
						ASkeletalMeshActor* actor_weapon = Cast<ASkeletalMeshActor>(GetWorld()->SpawnActor(templateBow->GetClass()));
						actor_weapon->GetSkeletalMeshComponent()->SetSkeletalMesh(skeletalMesh);
						mh_weapon = actor_weapon;
					}

					const USkeletalMeshSocket* sc = GetMesh()->GetSocketByName(*weapon->slot_equip);
					if (sc != NULL)
						sc->AttachActor(mh_weapon, GetMesh());
					else
						TRACE("sc == NULL  %s ", *weapon->slot_equip);

					//¸½¼þ
					staticMesh = Cast<UStaticMesh>(weapon->append_1.ToStringReference().TryLoad());
					if (mh_append == NULL)
					{
						mh_append = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
						mh_append->SetMobility(EComponentMobility::Movable);
					}
					mh_append->GetStaticMeshComponent()->SetStaticMesh(staticMesh);

					sc = GetMesh()->GetSocketByName(*weapon->slot1);
					if (sc != NULL)
						sc->AttachActor(mh_append, GetMesh());
					else
						TRACE("sc == NULL  %s ", *weapon->slot1);

					weapon_state = 1;
				}
			}
				break;
			case Fconfig_equip::SecondHand:
			{
				if (sh_weapon != NULL)
				{
					sh_weapon->Destroy();
					sh_weapon = NULL;
				}
				if (sh_append != NULL)
				{
					sh_append->Destroy();
					sh_append = NULL;
				}

				Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(equips[i], race);
				if (weapon != NULL)
				{
					UStaticMesh* staticMesh = Cast<UStaticMesh>(weapon->model.ToStringReference().TryLoad());
					if (staticMesh != NULL)
					{
						AStaticMeshActor* actor_weapon = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
						actor_weapon->GetStaticMeshComponent()->SetStaticMesh(staticMesh);
						actor_weapon->SetMobility(EComponentMobility::Movable);
						sh_weapon = actor_weapon;
					}

					const USkeletalMeshSocket* sc = GetMesh()->GetSocketByName(*weapon->slot_equip);
					if (sc != NULL)
						sc->AttachActor(sh_weapon, GetMesh());
					else
						TRACE("sc == NULL  %s ", *weapon->slot_equip);

					//¸½¼þ
					staticMesh = Cast<UStaticMesh>(weapon->append_1.ToStringReference().TryLoad());
					if (sh_append == NULL)
					{
						sh_append = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
						sh_append->SetMobility(EComponentMobility::Movable);
					}
					sh_append->GetStaticMeshComponent()->SetStaticMesh(staticMesh);

					sc = GetMesh()->GetSocketByName(*weapon->slot1);
					if (sc != NULL)
						sc->AttachActor(sh_append, GetMesh());
					else
						TRACE("sc == NULL  %s ", *weapon->slot1);

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
				GetMesh()->SetSkeletalMesh(DefaultHead);
				Head->SetSkeletalMesh(NULL);
			}

				break;
			case Fconfig_equip::Body:
				Body->SetSkeletalMesh(DefaultBody);
				break;
			case Fconfig_equip::Hand:
				Hand->SetSkeletalMesh(DefaultHand);
				break;
			case Fconfig_equip::Feet:
				Feet->SetSkeletalMesh(DefaultFeet);
				break;
			case Fconfig_equip::MainHand:
				//case EquipPos::RightHand:
			{
				if (mh_weapon != NULL)
				{
					mh_weapon->Destroy();
					mh_weapon = NULL;
				}
				if (mh_append != NULL)
				{
					mh_append->Destroy();
					mh_append = NULL;
				}
				weapon_state = 0;
			}
				break;
			case Fconfig_equip::SecondHand:
			{
				if (sh_weapon != NULL)
				{
					sh_weapon->Destroy();
					sh_weapon = NULL;
				}
				if (sh_append != NULL)
				{
					sh_append->Destroy();
					sh_append = NULL;
				}
			}
				break;
			default:
				break;
			}
		}
	}
}


void AMyCharacter::OpenWeapon()
{
	if (weapon_state == 1)
		return;
	weapon_state = 1;
	
	{
		Fconfig_equip* equip = UMyGameSingleton::Get().FindEquip(equips[Fconfig_equip::MainHand]);
		Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(equips[Fconfig_equip::MainHand], race);
		if (weapon != NULL)
		{
			if (mh_weapon != NULL)
			{
				mh_weapon->DetachRootComponentFromParent();
				const USkeletalMeshSocket* sc = GetMesh()->GetSocketByName(*weapon->slot_equip);
				if (sc != NULL)
					sc->AttachActor(mh_weapon, GetMesh());
				else
					TRACE("sc == NULL  %s ", *weapon->slot_equip);

			}
		}
	}
	{
		Fconfig_equip* equip = UMyGameSingleton::Get().FindEquip(equips[Fconfig_equip::SecondHand]);
		Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(equips[Fconfig_equip::SecondHand], race);
		if (weapon != NULL)
		{
			if (sh_weapon != NULL)
			{
				sh_weapon->DetachRootComponentFromParent();
				const USkeletalMeshSocket* sc = GetMesh()->GetSocketByName(*weapon->slot_equip);
				if (sc != NULL)
					sc->AttachActor(sh_weapon, GetMesh());
				else
					TRACE("sc == NULL  %s ", *weapon->slot_equip);

			}
		}
	}

	//UpdateAnimBP();
}

void AMyCharacter::CloseWeapon()
{
	if (weapon_state == 0)
		return;
	weapon_state = 0;

	{
		Fconfig_equip* equip = UMyGameSingleton::Get().FindEquip(equips[Fconfig_equip::MainHand]);
		Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(equips[Fconfig_equip::MainHand], race);
		if (weapon != NULL)
		{
			if (mh_weapon != NULL)
			{
				mh_weapon->DetachRootComponentFromParent();
				const USkeletalMeshSocket* sc = GetMesh()->GetSocketByName(*weapon->slot1);
				if (sc != NULL)
					sc->AttachActor(mh_weapon, GetMesh());
				else
					TRACE("sc == NULL  %s ", *weapon->slot1);

			}
		}
	}
	{
		Fconfig_equip* equip = UMyGameSingleton::Get().FindEquip(equips[Fconfig_equip::SecondHand]);
		Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(equips[Fconfig_equip::SecondHand], race);
		if (weapon != NULL)
		{
			if (sh_weapon != NULL)
			{
				sh_weapon->DetachRootComponentFromParent();
				const USkeletalMeshSocket* sc = GetMesh()->GetSocketByName(*weapon->slot1);
				if (sc != NULL)
					sc->AttachActor(sh_weapon, GetMesh());
				else
					TRACE("sc == NULL  %s ", *weapon->slot1);

			}
		}
	}

	//UpdateAnimBP();
}


void AMyCharacter::UpdateAnimBP()
{
	if (weapon_state == 0)
	{
		GetMesh()->SetAnimInstanceClass(DefaultAnimBP);
	}
	else
	{
		if (!equips[Fconfig_equip::MainHand].IsNone())
		{
			Fconfig_weapon_map* weaponMap = UMyGameSingleton::Get().FindWeaponMap(equips[Fconfig_equip::MainHand], race);
			
			if (weaponMap->anim_equip.Get() == NULL)
				weaponMap->anim_equip.ToStringReference().TryLoad();
			if (weaponMap->anim_unequip.Get() == NULL)
				weaponMap->anim_unequip.ToStringReference().TryLoad();
			if (weaponMap->anim_bp.Get() == NULL)
				weaponMap->anim_bp.ToStringReference().TryLoad();

			if (weaponMap->anim_bp.Get() != NULL)
				GetMesh()->SetAnimInstanceClass(weaponMap->anim_bp.Get()->GetAnimBlueprintGeneratedClass());
		}
		else
		{
			GetMesh()->SetAnimInstanceClass(DefaultAnimBP);
		}
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