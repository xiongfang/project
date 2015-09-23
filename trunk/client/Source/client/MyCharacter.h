// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Animation/SkeletalMeshActor.h"
#include "Skill.h"
#include "GameBattler.h"
#include "MyCharacter.generated.h"



class UWeaponBase;


UENUM()
enum class ActionState :uint8
{
	Idle,	//
	Move,	//移动
	Jump,	//跳跃
	Block,	//防御
	Attack,	//攻击
	EquipWeapon,	//装备武器
	Dead
};

UCLASS()
class CLIENT_API AMyCharacter : public AGameBattler
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

	//UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	//int32 hp;
	//UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	//int32 mp;
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	int32 level;
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	int32 exp;
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	FName class_type;

	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadOnly)
	TArray<FName> equips;

	UPROPERTY()
	TMap<FName,int32> items;

	UPROPERTY()
	TMap<FName, UTask*> tasks;


	int32 base_maxhp() override;
	int32 base_maxmp() override;
	int32 base_patk() override;
	int32 base_matk() override;
	int32 base_pdef() override;
	int32 base_mdef() override;


	UFUNCTION(Category = Logic, BlueprintCallable)
	void Equip(FName id);
	UFUNCTION(Category = Logic, BlueprintCallable)
	void UnEquip(int32 slot);

	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadWrite)
		ActionState State;

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
		void AnimNofity_TakeArrow();

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

	//任务
	UFUNCTION(Category = Logic, BlueprintCallable)
		void TaskAdd(FName id);
	UFUNCTION(Category = Logic, BlueprintCallable)
		void TaskFinish(FName id);
	UFUNCTION(Category = Logic, BlueprintCallable)
		void TaskReward(FName id);

	void SkillEffect(AGameBattler* User, USkill* skill) override;

	void AnimNofity_Shoot() override;

	bool can_move()override;
	bool can_use_skill()override;

	//相机控制
	USpringArmComponent* SprintArm;
	FVector2D oldTouchPositions[2];
	FVector2D oldTouchVector;
	bool fingerState[2];
	float oldTouchDistance;
}; 
