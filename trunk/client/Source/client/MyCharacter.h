// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Animation/SkeletalMeshActor.h"
#include "MyCharacter.generated.h"



class UWeaponBase;

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

	//FName DefaultAnimGroup;

	UPROPERTY()
	TArray<USkeletalMesh*> DefaultBodyMeshes;

	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Head;
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* Body;
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* Hand;
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* Feet;

	UPROPERTY()
	TArray<USkeletalMeshComponent*> Bodys;

	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
	TArray<UWeaponBase*> Weapons;
	
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
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	FName class_type;

	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	TArray<FName> equips;

	TMap<FName,int32> items;

	UPROPERTY(Category = Data, VisibleAnywhere,BlueprintReadOnly)
	TArray<FName> skills;

	

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


	UFUNCTION(Category = Logic, BlueprintCallable)
	void Equip(FName id);
	UFUNCTION(Category = Logic, BlueprintCallable)
	void UnEquip(int32 slot);



	//当前的动画状态
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		FName anim_group;
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		TAssetPtr<UAnimMontage> anim_openweapon;
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		TAssetPtr<UAnimMontage> anim_closeweapon;


	UFUNCTION(Category = Logic, BlueprintCallable)
		bool IsWeaponOpen();
	UFUNCTION(Category = Logic, BlueprintCallable)
		void ChangeWeaponState();

	UFUNCTION(Category = Logic, BlueprintCallable)
	void Attack();

	void UpdateMesh();
	void UpdateAnimGroup();


	//道具
	UFUNCTION(Category = Logic, BlueprintCallable)
		int32 ItemAdd(FName id, int32 count);

	UFUNCTION(Category = Logic, BlueprintCallable)
		int32 ItemLose(FName id, int32 count);

	UFUNCTION(Category = Logic, BlueprintCallable)
		int32 ItemMax(FName id);

	UFUNCTION(Category = Logic, BlueprintCallable)
		bool ItemEnough(FName id, int32 count);

	UFUNCTION()
	void OnActorOverlap(AActor* OtherActor);
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