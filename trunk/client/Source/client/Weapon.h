#pragma once
#include "config.h"
#include "Weapon.generated.h"


class AGameCharacter;
class AWeaponActor;

UCLASS()
class CLIENT_API UWeaponBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	AGameCharacter* Owner;
	FName _id;
public:
	virtual void OnEquip(FName id){ _id = id; }
	virtual void OnUnEquip(){ _id = NAME_None; }

	//拿出
	virtual void Open(){}
	//收起
	virtual void Close(){}

	FName GetID(){ return _id; }

	virtual void AttackStart(){}
	virtual void AttackEnd(){}

	virtual void PlayAnim(UAnimMontage* anim){};

	static UWeaponBase* Create(AGameCharacter* Parent, WeaponType weaponType);
};

UCLASS()
class CLIENT_API UWeaponSword : public UWeaponBase
{
	GENERATED_BODY()

protected:
	FName WeaponSlotName;
	FName AppendSlotName;
public:
	UWeaponSword();

	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
	AWeaponActor* mh_weapon;  //网格模型
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
	AStaticMeshActor* mh_append; //剑鞘

	virtual void OnEquip(FName id);
	virtual void OnUnEquip();
	//拿出
	virtual void Open();
	//收起
	virtual void Close();
};

UCLASS()
class CLIENT_API UWeaponLongSword : public UWeaponSword
{
	GENERATED_BODY()
public:
	UWeaponLongSword();
};

UCLASS()
class CLIENT_API UWeaponBow : public UWeaponBase
{
	GENERATED_BODY()

	FName WeaponSlotName;
	FName WeaponBackName;
	FName AppendSlotName;
public:
	UWeaponBow();

	//弓 索引
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		AWeaponActor* mh_weapon;
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		AStaticMeshActor* mh_append; //箭袋
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		ASkeletalMeshActor* mh_fly_arrow; //箭

	virtual void OnEquip(FName id);
	virtual void OnUnEquip();
	//拿出
	virtual void Open();
	//收起
	virtual void Close();

	virtual void AttackStart();
	virtual void AttackEnd();
	virtual void PlayAnim(UAnimMontage* anim);
};