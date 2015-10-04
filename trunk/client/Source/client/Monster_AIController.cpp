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
		//记录出生点
		home_location = Monster->GetActorLocation();
	}
	else
	{
		Monster = NULL;
	}
}

void AMonster_AIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Monster!=NULL && !Monster->IsDead())
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

	//如果无目标，选择目标
	if (Monster->Target == NULL)
	{
		TArray<AGameBattler*> battlers = Monster->FindBattlers(Monster->attention_range());
		AGameBattler* target = SelectTarget(battlers);
		Monster->SelectTarget(target);
	}

	//如果没有选择到目标或者目标死亡，回到出生点
	if (Monster->Target == NULL || Monster->Target->IsDead())
	{
		Monster->SelectTarget(NULL);
		this->MoveToLocation(home_location);
		return;
	}

	//计算最短可施法的距离
	TArray<USkill*> skills = Monster->GetSkills();
	float dist_min = 1000000.0f;
	bool has_usefull_skill = false;

	float dist = FVector::Dist(Monster->Target->GetActorLocation(), Monster->GetActorLocation());
	for (auto skill : skills)
	{
		//如果可攻击，直接攻击
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
	
	//如果没有技能攻击，移动接近目标
	if (dist > (dist_min-50.0f))
	{
		this->MoveToActor(Monster->Target);
		return;
	}
}

void AMonster_AIController::OnHit(AGameBattler* User, USkill* skill)
{
	if (Monster != NULL && Monster->Target == NULL && !Monster->IsDead())
	{
		Monster->Target = User;
	}
}