#include "client.h"
#include "Weapon.h"
#include "MyCharacter.h"
#include "MyGameSingleton.h"

UWeaponBase* UWeaponBase::Create(AMyCharacter* Parent, WeaponType type)
{
	switch (type)
	{
	case WeaponType::Sword:
	{
		UWeaponBase* Base = NewObject<UWeaponSword>(Parent,TEXT("WeaponSword"));
		Base->Owner = Parent;
		Base->RegisterComponent();
		return Base;
	}
		break;
	case WeaponType::LongSword:
	{
		UWeaponBase* Base = NewObject<UWeaponLongSword>(Parent, TEXT("WeaponLongSword"));
		Base->Owner = Parent;
		Base->RegisterComponent();
		return Base;
	}
		break;
	case WeaponType::Bow:
	{
		UWeaponBase* Base = NewObject<UWeaponBow>(Parent, TEXT("WeaponBow"));
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
		if (mh_weapon == NULL)
		{
			mh_weapon = Cast<AStaticMeshActor>(GetWorld()->SpawnActor(*weapon->prefab));
			mh_weapon->SetMobility(EComponentMobility::Movable);
		}

		const USkeletalMeshSocket* sc = Owner->GetMesh()->GetSocketByName(WeaponSlotName);
		if (sc != NULL)
			sc->AttachActor(mh_weapon, Owner->GetMesh());
		else
			TRACE("sc == NULL  %s ", *WeaponSlotName.ToString());
	
		//附件
		UStaticMesh* staticMesh = Cast<UStaticMesh>(weapon->append_1.ToStringReference().TryLoad());
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

UWeaponBow::UWeaponBow()
{
	WeaponSlotName = TEXT("SHIELD");
	WeaponBackName = TEXT("WeaponBow");
	AppendSlotName = TEXT("QUIVER");
}

void UWeaponBow::OnEquip(FName id)
{
	Super::OnEquip(id);

	Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(id, Owner->race);
	if (weapon != NULL)
	{
		if (mh_weapon == NULL)
		{
			mh_weapon = Cast<ASkeletalMeshActor>(GetWorld()->SpawnActor(*weapon->prefab));
		}

		const USkeletalMeshSocket* sc = Owner->GetMesh()->GetSocketByName(WeaponSlotName);
		if (sc != NULL)
			sc->AttachActor(mh_weapon, Owner->GetMesh());
		else
			TRACE("sc == NULL  %s ", *WeaponSlotName.ToString());

		//附件
		UStaticMesh* staticMesh = Cast<UStaticMesh>(weapon->append_1.ToStringReference().TryLoad());
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
void UWeaponBow::OnUnEquip()
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
void UWeaponBow::Open()
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
void UWeaponBow::Close()
{
	Fconfig_equip* equip = UMyGameSingleton::Get().FindEquip(_id);
	Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(_id, Owner->race);
	if (weapon != NULL)
	{
		if (mh_weapon != NULL)
		{
			mh_weapon->DetachRootComponentFromParent();
			const USkeletalMeshSocket* sc = Owner->GetMesh()->GetSocketByName(WeaponBackName);
			if (sc != NULL)
				sc->AttachActor(mh_weapon, Owner->GetMesh());
		}
	}

	Super::Close();
}

void UWeaponBow::AttackStart()
{
	//特殊处理一下箭
	Fconfig_weapon_map* wp = UMyGameSingleton::Get().FindWeaponMap(_id, Owner->race);
	if (wp != NULL && wp->append_3 != NULL)
	{
		USkeletalMesh* skeletalMesh = Cast<USkeletalMesh>(wp->append_3.ToStringReference().TryLoad());
		mh_fly_arrow = Owner->GetWorld()->SpawnActor<ASkeletalMeshActor>();
		mh_fly_arrow->GetSkeletalMeshComponent()->SetSkeletalMesh(skeletalMesh);
		const USkeletalMeshSocket* sc = Owner->GetMesh()->GetSocketByName("WEAPON");
		if (sc != NULL)
			sc->AttachActor(mh_fly_arrow, Owner->GetMesh());
	}
}
void UWeaponBow::AttackEnd()
{
	if (mh_fly_arrow != NULL)
	{
		mh_fly_arrow->Destroy();
		mh_fly_arrow = NULL;
	}
		
	////特殊处理一下箭
	//Fconfig_weapon_map* wp = UMyGameSingleton::Get().FindWeaponMap(_id, Owner->race);
	//if (wp != NULL && wp->append_3 != NULL)
	//{
	//	USkeletalMesh* skeletalMesh = Cast<USkeletalMesh>(wp->append_3.ToStringReference().TryLoad());
	//	ASkeletalMeshActor* mh_weapon = GetWorld()->SpawnActor<ASkeletalMeshActor>();
	//	mh_weapon->GetSkeletalMeshComponent()->SetSkeletalMesh(skeletalMesh);
	//	mh_weapon->SetOwner(this);
	//	const USkeletalMeshSocket* sc = GetMesh()->GetSocketByName("WEAPON");
	//	//if (sc != NULL)
	//	//	sc->
	//}
}

void UWeaponBow::PlayAnim(UAnimMontage* anim)
{
	UAnimInstance* animInst = mh_weapon->GetSkeletalMeshComponent()->GetAnimInstance();
	if (animInst!=NULL)
		animInst->Montage_Play(anim);
}