// Fill out your copyright notice in the Description page of Project Settings.

#include "client.h"
#include "GameNPC.h"
#include "MyGameSingleton.h"
#include "config.h"
#include "GameCharacter.h"

// Sets default values
AGameNPC::AGameNPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static FName CollisionProfileName(TEXT("IgnoreOnlyPawn"));

	Body = CreateOptionalDefaultSubobject< USkeletalMeshComponent>(TEXT("Body"));
	if (Body)
	{
		Body->AlwaysLoadOnClient = true;
		Body->AlwaysLoadOnServer = true;
		Body->bOwnerNoSee = false;
		Body->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
		Body->bCastDynamicShadow = true;
		Body->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Body->bChartDistanceFactor = true;
		Body->SetCollisionProfileName(CollisionProfileName);
		Body->bGenerateOverlapEvents = false;

		// Mesh acts as the head, as well as the parent for both animation and attachment.
		Body->AttachParent = GetMesh();
		Body->SetMasterPoseComponent(GetMesh());
	}

	Head = CreateOptionalDefaultSubobject< USkeletalMeshComponent>(TEXT("Head"));
	if (Head)
	{
		Head->AlwaysLoadOnClient = true;
		Head->AlwaysLoadOnServer = true;
		Head->bOwnerNoSee = false;
		Head->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
		Head->bCastDynamicShadow = true;
		Head->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Head->bChartDistanceFactor = true;
		Head->SetCollisionProfileName(CollisionProfileName);
		Head->bGenerateOverlapEvents = false;

		// Mesh acts as the head, as well as the parent for both animation and attachment.
		Head->AttachParent = GetMesh();
		Head->SetMasterPoseComponent(GetMesh());

	}
	Hand = CreateOptionalDefaultSubobject< USkeletalMeshComponent>(TEXT("Hand"));
	if (Hand)
	{
		Hand->AlwaysLoadOnClient = true;
		Hand->AlwaysLoadOnServer = true;
		Hand->bOwnerNoSee = false;
		Hand->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
		Hand->bCastDynamicShadow = true;
		Hand->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Hand->bChartDistanceFactor = true;
		Hand->SetCollisionProfileName(CollisionProfileName);
		Hand->bGenerateOverlapEvents = false;

		// Mesh acts as the head, as well as the parent for both animation and attachment.
		Hand->AttachParent = GetMesh();
		Hand->SetMasterPoseComponent(GetMesh());

	}
	Feet = CreateOptionalDefaultSubobject< USkeletalMeshComponent>(TEXT("Feet"));
	if (Feet)
	{
		Feet->AlwaysLoadOnClient = true;
		Feet->AlwaysLoadOnServer = true;
		Feet->bOwnerNoSee = false;
		Feet->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
		Feet->bCastDynamicShadow = true;
		Feet->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Feet->bChartDistanceFactor = true;
		Feet->SetCollisionProfileName(CollisionProfileName);
		Feet->bGenerateOverlapEvents = false;

		// Mesh acts as the head, as well as the parent for both animation and attachment.
		Feet->AttachParent = GetMesh();
		Feet->SetMasterPoseComponent(GetMesh());
	}

	talk_range = 300.0f;
}


TArray<UTask*> AGameNPC::GetTasks(AGameCharacter* c)
{
	TArray<FName> taskList = UMyGameSingleton::Get().config_task->GetRowNames();
	TArray<UTask*> result;
	for (auto taskId : taskList)
	{
		UTask* task = c->TaskGet(taskId);
		if (task != NULL)
		{
			Fconfig_task* taskData = UMyGameSingleton::Get().FindTask(taskId);

			if (c->level < taskData->level)
				continue;

			if (taskData->npc_start == name())
			{
				if (task->State == UTask::TaskState::NoStart || task->State == UTask::TaskState::Start)
				{
					result.AddUnique(task);
				}
			}

			if (taskData->npc_finish == name())
			{
				if (task->State == UTask::TaskState::Finish)
				{
					result.AddUnique(task);
				}
			}
		}
	}
	return result;
}

void AGameNPC::Event_OnSelect_Implementation(AGameBattler* User)
{
	character = Cast<AGameCharacter>(User);
	if (character == NULL)
		return;

	if (FVector::Dist(User->GetActorLocation(), GetActorLocation()) < talk_range)
	{
		character->OpenDialog();
		TArray<FText> selections;
		TArray<UTask*> npcTasks = GetTasks(character);
		for (auto task : npcTasks)
		{
			if (task->can_finish(character))
				character->TaskFinish(task->id);

			selections.Add(FText::FromName(task->id));
		}
		selections.Add(FText::FromString(FString(TEXT("再见"))));
		character->ShowText(TEXT("start"), FText::FromString(TEXT("需要帮助吗？")), selections);
	}
}

void AGameNPC::OnDialogSelect(const FString& token, int32 index)
{
	if (task != NULL)
	{
		sentence_index++;
		task->Event_OnDialogSelect(this, character, token, index);
	}
	else
	{
		TArray<UTask*> npcTasks = GetTasks(character);

		if (npcTasks.IsValidIndex(index))
		{
			task = npcTasks[index];
			sentence_index = 0;
		}
	}

	if (task == NULL)
	{
		character->CloseDialog();
		task = NULL;
		character = NULL;
		sentence_index = 0;
		return;
	}

	Fconfig_task* taskData = UMyGameSingleton::Get().FindTask(task->id);

	switch (task->State)
	{
	case UTask::TaskState::NoStart :
	{
		if (taskData->dialog_start.IsValidIndex(sentence_index))
		{
			character->ShowText(taskData->dialog_start[sentence_index].token, taskData->dialog_start[sentence_index].text, taskData->dialog_start[sentence_index].selections);
		}
		else
		{
			character->CloseDialog();
			task = NULL;
			character = NULL;
			sentence_index = 0;
		}
		break;
	}
	case UTask::TaskState::Start :
	{
		if (taskData->dialog_going.IsValidIndex(sentence_index))
		{
			character->ShowText(taskData->dialog_going[sentence_index].token, taskData->dialog_going[sentence_index].text, taskData->dialog_going[sentence_index].selections);
		}
		else
		{
			character->CloseDialog();
			task = NULL;
			character = NULL;
			sentence_index = 0;
		}
		break;
	}
	case UTask::TaskState::Finish :
	{
		if (taskData->dialog_finish.IsValidIndex(sentence_index))
		{
			character->ShowText(taskData->dialog_finish[sentence_index].token, taskData->dialog_finish[sentence_index].text, taskData->dialog_finish[sentence_index].selections);
		}
		else
		{
			character->CloseDialog();
			task = NULL;
			character = NULL;
			sentence_index = 0;
		}
		break;
	}
	default:
		character->CloseDialog();
		task = NULL;
		character = NULL;
		sentence_index = 0;
		break;
	}
	
}