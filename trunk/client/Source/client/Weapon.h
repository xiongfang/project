#pragma once

#include "Weapon.generated.h"

class AMyCharacter;

UCLASS()
class CLIENT_API UWeaponBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	AMyCharacter* Owner;
	FName _id;
	bool _isOpened;
public:
	virtual void OnEquip(FName id){ _id = id; }
	virtual void OnUnEquip(){ _id = NAME_None; }

	//�Ƿ��ó�״̬
	virtual bool IsOpen(){ return _isOpened; }
	//�ó�
	virtual void Open(){ _isOpened = true; }
	//����
	virtual void Close(){ _isOpened = false; }

	FName GetID(){ return _id; }

	static UWeaponBase* Create(AMyCharacter* Parent, int32 weaponType);
};

UCLASS()
class CLIENT_API UWeaponSword : public UWeaponBase
{
	GENERATED_BODY()
public:
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
	AStaticMeshActor* mh_weapon;  //����ģ��
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
	AStaticMeshActor* mh_append; //����

	virtual void OnEquip(FName id);
	virtual void OnUnEquip();
	//�ó�
	virtual void Open();
	//����
	virtual void Close();
};

UCLASS()
class CLIENT_API UWeaponDoubleHand : public UWeaponBase
{
	GENERATED_BODY()
public:
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		AStaticMeshActor* mh_weapon;  //����ģ��
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		AStaticMeshActor* mh_append; //����

	virtual void OnEquip(FName id);
	virtual void OnUnEquip();
	//�ó�
	virtual void Open();
	//����
	virtual void Close();
};