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
		UWeaponBase* Base = NewObject<UWeaponLongSword>(Parent, TEXT("WeaponLongSword"));
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


UWeaponSword::UWeaponSword()
{
	WeaponSlotName = TEXT("WEAPON");
	AppendSlotName = TEXT("WeaponSword");
}
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

		const USkeletalMeshSocket* sc = Owner->GetMesh()->GetSocketByName(WeaponSlotName);
		if (sc != NULL)
			sc->AttachActor(mh_weapon, Owner->GetMesh());
		else
			TRACE("sc == NULL  %s ", *WeaponSlotName.ToString());
	
		//附件
		staticMesh = Cast<UStaticMesh>(weapon->append_1.ToStringReference().TryLoad());
		if (mh_append == NULL)
		{
			mh_append = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
			mh_append->SetMobility(EComponentMobility::Movable);
		}
		mh_append->GetStaticMeshComponent()->SetStaticMesh(staticMesh);
	
		sc = Owner->GetMesh()->GetSocketByName(AppendSlotName);
		if (sc != NULL)
			sc->AttachActor(mh_append, Owner->GetMesh());
		else
			TRACE("sc == NULL  %s ", *AppendSlotName.ToString());

		_isOpened = true;

		//禁用碰撞
		UPrimitiveComponent* Box = mh_weapon->FindComponentByClass<UPrimitiveComponent>();
		if (Box != NULL)
			Box->bGenerateOverlapEvents = false;
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
			const USkeletalMeshSocket* sc = Owner->GetMesh()->GetSocketByName(WeaponSlotName);
			if (sc != NULL)
				sc->AttachActor(mh_weapon, Owner->GetMesh());
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
			const USkeletalMeshSocket* sc = Owner->GetMesh()->GetSocketByName(AppendSlotName);
			if (sc != NULL)
				sc->AttachActor(mh_weapon, Owner->GetMesh());
		}
	}

	Super::Close();
}


UWeaponLongSword::UWeaponLongSword()
{
	WeaponSlotName = TEXT("WEAPON");
	AppendSlotName = TEXT("WeaponBack");
}