// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Animation/SkeletalMeshActor.h"
#include "Skill.h"
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
	int32 mp;
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

	UPROPERTY(Category = Data, VisibleAnywhere)
	TMap<FName,USkill*> skills;

	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
	AMyCharacter* Target;  //当前选择的目标


	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
	float skill_common_cd; //公共CD
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
	USkill* current_skill;//当前正在使用的技能

	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadWrite)
	ActionState State;

	UFUNCTION(Category = Logic, BlueprintCallable)
	void LearnSkill(FName skillId);
	

	UFUNCTION(Category = Logic, BlueprintCallable)
	void Recover();

	UFUNCTION(Category = Logic, BlueprintCallable)
	int32 maxhp();
	UFUNCTION(Category = Logic, BlueprintCallable)
	int32 maxmp();
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
		bool CanMove();
	UFUNCTION(Category = Logic, BlueprintCallable)
		bool CanUseSkill();

	UFUNCTION(Category = Logic, BlueprintCallable)
		bool CanUseSkillTarget(FName skill);

	UFUNCTION(Category = Logic, BlueprintCallable)
		void Attack(FName skillId);

	UFUNCTION(Category = Logic, BlueprintCallable)
		void SkillEffect(AMyCharacter* User, USkill* skill);

	UFUNCTION(BlueprintImplementableEvent)
		void ReceiveSkillEffect(AMyCharacter* User, USkill* skill);

	UFUNCTION(Category = Logic, BlueprintCallable)
		void AnimNofity_SkillEffect();

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


	UFUNCTION(Category = Logic, BlueprintCallable)
	void SelectTarget(AMyCharacter* t);
};
