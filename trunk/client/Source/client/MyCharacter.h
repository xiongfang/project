// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Animation/SkeletalMeshActor.h"
#include "MyCharacter.generated.h"


UCLASS()
class CLIENT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void PostInitializeComponents() override;

	UPROPERTY()
	USkeletalMesh* DefaultHead;
	UPROPERTY()
	USkeletalMesh* DefaultBody;
	UPROPERTY()
	USkeletalMesh* DefaultHand;
	UPROPERTY()
	USkeletalMesh* DefaultFeet;

	UPROPERTY(Category = Anim, EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> DefaultAnimBP;
	

	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Head;

	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Body;
	
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Hand;

	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Feet;

	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
	AActor* mh_weapon;  //��̬����ģ�ͻ��߹�������ģ��
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
	AStaticMeshActor* mh_append;

	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
	AStaticMeshActor* sh_weapon;
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
	AStaticMeshActor* sh_append;
	
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AStaticMeshActor> templateSword;
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASkeletalMeshActor> templateBow;

	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	int32 hp;
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	int32 level;
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	int32 exp;
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	FName race;


	UFUNCTION(Category = Logic, BlueprintCallable)
		void Reset();

	UFUNCTION(Category = Logic, BlueprintCallable)
		int32 maxhp();
	UFUNCTION(Category = Logic, BlueprintCallable)
		int32 patk();
	UFUNCTION(Category = Logic, BlueprintCallable)
		int32 matk();
	UFUNCTION(Category = Logic, BlueprintCallable)
		int32 pdef();
	UFUNCTION(Category = Logic, BlueprintCallable)
		int32 mdef();

	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	TArray<FName> equips;

	TMap<FName,int32> items;


	UFUNCTION(Category = Logic, BlueprintCallable)
	FName main_weapon_map();

	UFUNCTION(Category = Logic, BlueprintCallable)
	void Equip(int32 slot, FName id);
	
	UFUNCTION(Category = Logic, BlueprintCallable)
		void OpenWeapon();
	UFUNCTION(Category = Logic, BlueprintCallable)
		void CloseWeapon();

	//0 δװ����1װ��
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		int32 weapon_state;  

	UFUNCTION(Category = Logic, BlueprintCallable)
	void UpdateAnimBP();

	//UFUNCTION(Category = Logic,BlueprintCallable)
	void UpdateMesh();
	//void LoadWeaponAnimations();


	//����
	UFUNCTION(Category = Logic, BlueprintCallable)
		int32 ItemAdd(FName id, int32 count);

	UFUNCTION(Category = Logic, BlueprintCallable)
		int32 ItemLose(FName id, int32 count);

	UFUNCTION(Category = Logic, BlueprintCallable)
		int32 ItemMax(FName id);

	UFUNCTION(Category = Logic, BlueprintCallable)
		bool ItemEnough(FName id, int32 count);


};


enum SocketType
{
	WeaponDagger,
	WeaponAxe,
	WeaponSword,
	WeaponMace,
	WEAPON,
	AnimObjectR,
	SHIELD,
	AnimObjectL,
	WeaponBack,
	WeaponBow,
	QUIVER
};