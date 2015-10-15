#include "client.h"
#include "Weapon.h"
#include "GameCharacter.h"
#include "MyGameSingleton.h"
#include "WeaponActor.h"

UWeaponBase* UWeaponBase::Create(AGameCharacter* Parent, WeaponType type)
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

	Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(id, Owner->race());
	if (weapon == NULL)
	{
		UE_LOG(client, Warning, TEXT("%s,%s WeaponMap Not Found!"), *id.ToString(), *Owner->race().ToString());
		return;
	}

	if (weapon->static_meshes.Num()==0)
	{
		UE_LOG(client, Warning, TEXT("%s,%s weapon static_meshes not set"), *id.ToString(), *Owner->race().ToString());
		return;
	}

	UStaticMesh* mesh = weapon->static_meshes.Num() > 0 ? weapon->static_meshes[0].LoadSynchronous() : NULL;
	if (mh_weapon == NULL)
	{
		mh_weapon = Cast<AWeaponActor>(GetWorld()->SpawnActor(*weapon->prefab));
		if (mh_weapon == NULL)
		{
			UE_LOG(client, Warning, TEXT("%s,%s weapon prefab not set"), *id.ToString(), *Owner->race().ToString());
			return;
		}
	}
	mh_weapon->GetStaticMeshComponent()->SetStaticMesh(mesh);

	//const USkeletalMeshSocket* sc = Owner->GetMesh()->GetSocketByName(WeaponSlotName);
	//if (sc != NULL)
	//	sc->AttachActor(mh_weapon, Owner->GetMesh());
	//else
	//	TRACE("sc == NULL  %s ", *WeaponSlotName.ToString());
	
	//附件
	UStaticMesh* staticMesh = weapon->static_meshes.Num() > 1 ? weapon->static_meshes[1].LoadSynchronous() : NULL;
	if (mh_append == NULL)
	{
		mh_append = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
		mh_append->SetMobility(EComponentMobility::Movable);
	}
	mh_append->GetStaticMeshComponent()->SetStaticMesh(staticMesh);
	
	const USkeletalMeshSocket* sc = Owner->GetMesh()->GetSocketByName(AppendSlotName);
	if (sc != NULL)
		sc->AttachActor(mh_append, Owner->GetMesh());
	else
		TRACE("sc == NULL  %s ", *AppendSlotName.ToString());

	//禁用碰撞
	UPrimitiveComponent* Box = mh_weapon->FindComponentByClass<UPrimitiveComponent>();
	if (Box != NULL)
		Box->bGenerateOverlapEvents = false;

	//更新装备状态
	if (Owner->is_weapon_open)
	{
		Open();
	}
	else
	{
		Close();
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
	Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(_id, Owner->race());
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
	Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(_id, Owner->race());
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

	Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(id, Owner->race());
	if (weapon == NULL)
	{
		UE_LOG(client, Warning, TEXT("%s,%s WeaponMap Not Found!"), *id.ToString(), *Owner->race().ToString());
		return;
	}

	if (weapon->skeletal_meshes.Num() < 2)
	{
		UE_LOG(client, Warning, TEXT("%s,%s weapon skeletal_meshes is required"), *id.ToString(), *Owner->race().ToString());
		return;
	}

	USkeletalMesh* skeletalMesh = weapon->skeletal_meshes[0].LoadSynchronous();
	if (mh_weapon == NULL)
	{
		mh_weapon = Cast<AWeaponActor>(GetWorld()->SpawnActor(*weapon->prefab));
		if (mh_weapon == NULL)
		{
			UE_LOG(client, Warning, TEXT("%s,%s weapon prefab not set"), *id.ToString(), *Owner->race().ToString());
			return;
		}
	}
	mh_weapon->GetSkeletalMeshComponent()->SetSkeletalMesh(skeletalMesh);

	//const USkeletalMeshSocket* sc = Owner->GetMesh()->GetSocketByName(WeaponSlotName);
	//if (sc != NULL)
	//	sc->AttachActor(mh_weapon, Owner->GetMesh());
	//else
	//	TRACE("sc == NULL  %s ", *WeaponSlotName.ToString());

	//附件
	UStaticMesh* staticMesh = weapon->static_meshes.Num() > 0 ? weapon->static_meshes[0].LoadSynchronous() : NULL;
	if (mh_append == NULL)
	{
		mh_append = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
		mh_append->SetMobility(EComponentMobility::Movable);
	}
	mh_append->GetStaticMeshComponent()->SetStaticMesh(staticMesh);

	const USkeletalMeshSocket* sc = Owner->GetMesh()->GetSocketByName(AppendSlotName);
	if (sc != NULL)
		sc->AttachActor(mh_append, Owner->GetMesh());
	else
		TRACE("sc == NULL  %s ", *AppendSlotName.ToString());

	//禁用碰撞
	UPrimitiveComponent* Box = mh_weapon->FindComponentByClass<UPrimitiveComponent>();
	if (Box != NULL)
		Box->bGenerateOverlapEvents = false;

	//更新装备状态
	if (Owner->is_weapon_open)
	{
		Open();
	}
	else
	{
		Close();
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
	Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(_id, Owner->race());
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
	Fconfig_weapon_map* weapon = UMyGameSingleton::Get().FindWeaponMap(_id, Owner->race());
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
	Fconfig_weapon_map* wp = UMyGameSingleton::Get().FindWeaponMap(_id, Owner->race());
	if (wp != NULL && wp->skeletal_meshes.Num()>=2)
	{
		USkeletalMesh* skeletalMesh = wp->skeletal_meshes[1].LoadSynchronous();
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