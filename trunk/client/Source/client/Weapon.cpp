#include "client.h"
#include "Weapon.h"
#include "config.h"

#include "MyCharacter.h"
#include "MyGameSingleton.h"

UWeaponBase* UWeaponBase::Create(AMyCharacter* Parent, int32 type)
{
	switch (type)
	{
	case Fconfig_equip::WeaponType::Sword:
	{
		UWeaponBase* Base = NewObject<UWeaponSword>(Parent,TEXT("WeaponSword"));
		Base->Owner = Parent;
		Base->RegisterComponent();
		return Base;
	}
		break;
	case Fconfig_equip::WeaponType::LongSword:
	{
		UWeaponBase* Base = NewObject<UWeaponDoubleHand>(Parent, TEXT("DoubleWeaponSword"));
		Base->Owner = Parent;
		Base->RegisterComponent();
		return Base;
	}
	break;
	default:
		break;
	}
	return NULL;
}


#define WeaponSwordSlotName TEXT("WEAPON")
#define WeaponAppendSlotName  TEXT("WeaponSword")

void UWeaponSword::OnEquip(FName id)
{
	Super::OnEquip(id);

	Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(id, Owner->race);
	if (weapon != NULL)
	{
						
		UStaticMesh* staticMesh = Cast<UStaticMesh>(weapon->model.ToStringReference().TryLoad());
		if (mh_weapon == NULL)
		{
			mh_weapon = Cast<AStaticMeshActor>(GetWorld()->SpawnActor(*Owner->templateSword));
			mh_weapon->SetMobility(EComponentMobility::Movable);
		}
		mh_weapon->GetStaticMeshComponent()->SetStaticMesh(staticMesh);

		const USkeletalMeshSocket* sc = Owner->GetMesh()->GetSocketByName(WeaponSwordSlotName);
		if (sc != NULL)
			sc->AttachActor(mh_weapon, Owner->GetMesh());
		else
			TRACE("sc == NULL  %s ", WeaponSwordSlotName);
	
		//附件
		staticMesh = Cast<UStaticMesh>(weapon->append_1.ToStringReference().TryLoad());
		if (mh_append == NULL)
		{
			mh_append = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
			mh_append->SetMobility(EComponentMobility::Movable);
		}
		mh_append->GetStaticMeshComponent()->SetStaticMesh(staticMesh);
	
		sc = Owner->GetMesh()->GetSocketByName(WeaponAppendSlotName);
		if (sc != NULL)
			sc->AttachActor(mh_append, Owner->GetMesh());
		else
			TRACE("sc == NULL  %s ", WeaponAppendSlotName);

		_isOpened = true;
	}

}
void UWeaponSword::OnUnEquip()
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
	Super::OnUnEquip();

	this->UnregisterComponent();
}


//拿出
void UWeaponSword::Open()
{
	Super::Open();

	Fconfig_equip* equip = UMyGameSingleton::Get().FindEquip(_id);
	Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(_id, Owner->race);
	if (weapon != NULL)
	{
		if (mh_weapon != NULL)
		{
			mh_weapon->DetachRootComponentFromParent();
			const USkeletalMeshSocket* sc = Owner->GetMesh()->GetSocketByName(WeaponSwordSlotName);
			if (sc != NULL)
				sc->AttachActor(mh_weapon, Owner->GetMesh());
			else
				TRACE("sc == NULL  %s ", WeaponSwordSlotName);
	
			//TScriptDelegate<FWeakObjectPtr> BindObject;
			//BindObject.BindUFunction(Owner, TEXT("OnActorOverlap"));
			//mh_weapon->OnActorBeginOverlap.Add(BindObject);
		}
	}


}
//收起
void UWeaponSword::Close()
{
	Fconfig_equip* equip = UMyGameSingleton::Get().FindEquip(_id);
	Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(_id, Owner->race);
	if (weapon != NULL)
	{
		if (mh_weapon != NULL)
		{
			mh_weapon->DetachRootComponentFromParent();
			const USkeletalMeshSocket* sc = Owner->GetMesh()->GetSocketByName(WeaponAppendSlotName);
			if (sc != NULL)
				sc->AttachActor(mh_weapon, Owner->GetMesh());
			else
				TRACE("sc == NULL  %s ", WeaponAppendSlotName);
	
		}
	}

	Super::Close();
}

#undef WeaponSwordSlotName
#undef WeaponAppendSlotName


#define DH_SlotName TEXT("WEAPON")
#define DH_AppendSlotName  TEXT("WeaponBack")

void UWeaponDoubleHand::OnEquip(FName id)
{
	Super::OnEquip(id);

	Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(id, Owner->race);
	if (weapon != NULL)
	{

		UStaticMesh* staticMesh = Cast<UStaticMesh>(weapon->model.ToStringReference().TryLoad());
		if (mh_weapon == NULL)
		{
			mh_weapon = Cast<AStaticMeshActor>(GetWorld()->SpawnActor(*Owner->templateSword));
			mh_weapon->SetMobility(EComponentMobility::Movable);
		}
		mh_weapon->GetStaticMeshComponent()->SetStaticMesh(staticMesh);

		const USkeletalMeshSocket* sc = Owner->GetMesh()->GetSocketByName(DH_SlotName);
		if (sc != NULL)
			sc->AttachActor(mh_weapon, Owner->GetMesh());
		else
			TRACE("sc == NULL  %s ", DH_SlotName);

		//附件
		staticMesh = Cast<UStaticMesh>(weapon->append_1.ToStringReference().TryLoad());
		if (mh_append == NULL)
		{
			mh_append = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
			mh_append->SetMobility(EComponentMobility::Movable);
		}
		mh_append->GetStaticMeshComponent()->SetStaticMesh(staticMesh);

		sc = Owner->GetMesh()->GetSocketByName(DH_AppendSlotName);
		if (sc != NULL)
			sc->AttachActor(mh_append, Owner->GetMesh());
		else
			TRACE("sc == NULL  %s ", DH_AppendSlotName);

		_isOpened = true;
	}

}
void UWeaponDoubleHand::OnUnEquip()
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
	Super::OnUnEquip();

	this->UnregisterComponent();
}


//拿出
void UWeaponDoubleHand::Open()
{
	Super::Open();

	Fconfig_equip* equip = UMyGameSingleton::Get().FindEquip(_id);
	Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(_id, Owner->race);
	if (weapon != NULL)
	{
		if (mh_weapon != NULL)
		{
			mh_weapon->DetachRootComponentFromParent();
			const USkeletalMeshSocket* sc = Owner->GetMesh()->GetSocketByName(DH_SlotName);
			if (sc != NULL)
				sc->AttachActor(mh_weapon, Owner->GetMesh());
			else
				TRACE("sc == NULL  %s ", DH_SlotName);

		}
	}


}
//收起
void UWeaponDoubleHand::Close()
{
	Fconfig_equip* equip = UMyGameSingleton::Get().FindEquip(_id);
	Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(_id, Owner->race);
	if (weapon != NULL)
	{
		if (mh_weapon != NULL)
		{
			mh_weapon->DetachRootComponentFromParent();
			const USkeletalMeshSocket* sc = Owner->GetMesh()->GetSocketByName(DH_AppendSlotName);
			if (sc != NULL)
				sc->AttachActor(mh_weapon, Owner->GetMesh());
			else
				TRACE("sc == NULL  %s ", DH_AppendSlotName);

		}
	}

	Super::Close();
}