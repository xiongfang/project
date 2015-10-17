// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Animation/SkeletalMeshActor.h"
#include "config.h"
#include "GameBattler.h"
#include "GameCharacter.generated.h"



class UWeaponBase;


#define USE_BODY_EQUIP	0

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
class CLIENT_API AGameCharacter : public AGameBattler
{
	GENERATED_BODY()
protected:
	virtual void NotifyEnterCombating()override;
	virtual void NotifyLeaveCombating()override;
public:
	// Sets default values for this character's properties
	AGameCharacter();

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
	
	//UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<AStaticMeshActor> templateSword;
	//UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<ASkeletalMeshActor> templateBow;

	//UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	//int32 hp;
	//UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	//int32 mp;
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	FName _name;
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	int32 level;
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	int32 exp;
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	FName class_type;
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
	FName race_type;
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
	float attention_range;

	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadOnly)
	TArray<FName> equips;

	UPROPERTY()
	TMap<FName,int32> items;

	UPROPERTY()
	TMap<FName, UTask*> tasks;

	FName name() override{ return _name; }
	int32 base_maxhp() override;
	int32 base_maxmp() override;
	int32 base_patk() override;
	int32 base_matk() override;
	int32 base_pdef() override;
	int32 base_mdef() override;
	int32 base_hit() override;
	int32 base_eva() override;
	FName race()override{ return race_type; }

	UFUNCTION(Category = Logic, BlueprintCallable)
	void Equip(FName id);
	UFUNCTION(Category = Logic, BlueprintCallable)
	void UnEquip(int32 slot);

	//UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadWrite)
	//	ActionState State;

	//当前的动画状态
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		FName anim_group;
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		UAnimMontage* anim_openweapon;
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		UAnimMontage* anim_closeweapon;
	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		UAnimSequence* anim_block;

	UPROPERTY(Category = Data, VisibleAnywhere, BlueprintReadOnly)
		bool is_weapon_open;

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
	UFUNCTION(Category = Logic, BlueprintCallable)
		UTask* TaskGet(FName id);

	bool Attack(FName skillId)override;
	void SkillEffect(AGameBattler* User, USkill* skill) override;

	void AnimNofity_Shoot() override;

	bool can_move()override;
	bool can_use_skill(USkill* skill)override;
	
	UFUNCTION(Category = Logic, BlueprintCallable)
	bool can_block();

	UFUNCTION(Category = Logic, BlueprintCallable)
		TArray<FItem> GetItems();


	UFUNCTION(Category = Logic, BlueprintCallable)
		TArray<UTask*> GetTasks();

	UFUNCTION(Category = Logic, BlueprintCallable)
		FString GetAttributeText();

	void SerializeProperty(FArchive& ar)override;


	UFUNCTION(Category = Logic, BlueprintCallable)
		void AddExp(int32 e);
	UFUNCTION(Category = Logic, BlueprintCallable)
		void LevelUp();

	void UpdateClassSkills();

	UFUNCTION(Category = Logic, BlueprintCallable)
		bool AutoSelectTarget(USkill* skill);

	UFUNCTION(Category = Logic, BlueprintImplementableEvent, BlueprintCallable)
		void OpenDialog();
	UFUNCTION(Category = Logic, BlueprintImplementableEvent, BlueprintCallable)
		void CloseDialog();
	UFUNCTION(Category = Logic, BlueprintImplementableEvent, BlueprintCallable)
		void ShowText(const FString& token, const FText& text, const TArray<FText>& selections);

}; 
