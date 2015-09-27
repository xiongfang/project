// Fill out your copyright notice in the Description page of Project Settings.

#include "client.h"
#include "Monster_AIController.h"
#include "GameMonster.h"
#include "Skill.h"
#include "config.h"

AMonster_AIController::AMonster_AIController()
{
	ai_tick_period = 0.5f;
	ai_tick_timer = 0.0f;
}

void AMonster_AIController::SetPawn(APawn* pawn)
{
	Super::SetPawn(pawn);

	if (pawn != NULL)
	{
		Monster = Cast<AGameMonster>(pawn);
		check(Monster);
		//��¼������
		home_location = Monster->GetActorLocation();
	}
	else
	{
		Monster = NULL;
	}
}


TArray<AGameBattler*> AMonster_AIController::GetAttentionBattlers()
{
	TArray<AGameBattler*> results;

	TArray<struct FOverlapResult> OutOverlaps;

	static const FName SphereTraceMultiName(TEXT("SphereTraceMulti"));

	FCollisionQueryParams Params(SphereTraceMultiName, false);
	Params.bReturnPhysicalMaterial = true;
	Params.bTraceAsyncScene = true;
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	if (ObjectParams.IsValid() == false)
	{
		UE_LOG(LogBlueprintUserMessages, Warning, TEXT("Invalid object types"));
		return results;
	}

	float Radius = Monster->attention_range();
	UWorld* World = GetWorld();
	bool const bHit = World->OverlapMultiByObjectType(OutOverlaps, Monster->GetActorLocation(), FQuat::Identity, ObjectParams, FCollisionShape::MakeSphere(Radius), Params);
	if (bHit)
	{
		for (auto hit : OutOverlaps)
		{
			AActor* hitActor = hit.Actor.Get();
			AGameBattler* battler = Cast<AGameBattler>(hitActor);
			if (battler != NULL)
			{
				results.Add(battler);
			}
		}
	}
	return results;
}


void AMonster_AIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Monster!=NULL)
		TickAI(DeltaSeconds);
}

AGameBattler* AMonster_AIController::SelectTarget_Implementation(const TArray<AGameBattler*>& battlers)
{
	for (auto battler : battlers)
	{
		if (battler->IsEnemy(Monster))
			return battler;
	}
	return NULL;
}

void AMonster_AIController::TickAI(float DeltaSeconds)
{
	ai_tick_timer += DeltaSeconds;
	if (ai_tick_timer <= 0.5f)
	{
		return;
	}
	ai_tick_timer = 0.0f;

	//�����Ŀ�꣬ѡ��Ŀ��
	if (Monster->Target == NULL)
	{
		TArray<AGameBattler*> battlers = GetAttentionBattlers();
		AGameBattler* target = SelectTarget(battlers);
		Monster->SelectTarget(target);
	}

	//���û��ѡ��Ŀ�����Ŀ���������ص�������
	if (Monster->Target == NULL || Monster->Target->IsDead())
	{
		Monster->SelectTarget(NULL);
		this->MoveToLocation(home_location);
		return;
	}

	//������̿�ʩ���ľ���
	TArray<USkill*> skills = Monster->GetSkills();
	float dist_min = 1000000.0f;
	bool has_usefull_skill = false;

	float dist = FVector::Dist(Monster->Target->GetActorLocation(), Monster->GetActorLocation());
	for (auto skill : skills)
	{
		//����ɹ�����ֱ�ӹ���
		if (Monster->can_use_skill_target(skill, Monster->Target))
		{
			Monster->Attack(skill->id);
			return;
		}

		if (skill->GetData()->distance < dist_min)
		{
			dist_min = skill->GetData()->distance;
		}
	}
	
	//���û�м��ܹ������ƶ��ӽ�Ŀ��
	if (dist > (dist_min-50.0f))
	{
		this->MoveToActor(Monster->Target);
		return;
	}
}