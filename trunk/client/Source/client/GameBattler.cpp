#include "client.h"
#include "GameBattler.h"
#include "Skill.h"
#include "config.h"

// Called every frame
void AGameBattler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//更新所有技能的CD
	for (auto kv : skills)
	{
		kv.Value->Update(DeltaTime);
	}
	//更新公共CD
	if (skill_common_cd > 0)
	{
		skill_common_cd -= DeltaTime;
		skill_common_cd = FMath::Max(0.0f, skill_common_cd);
	}

	TArray<FName> removedSkill;
	//更新所有状态的CD
	for (auto kv : states)
	{
		if (kv.Value->GetData()->time_schedule > 0)
		{
			kv.Value->schedule_timer += DeltaTime;
			if (kv.Value->schedule_timer >= kv.Value->GetData()->time_schedule)
			{
				kv.Value->ReceiveTimeSchedule(this);
				kv.Value->schedule_timer = 0.0f;
			}
		}

		kv.Value->cd -= DeltaTime;
		if (kv.Value->cd <= 0)
		{
			removedSkill.Add(kv.Key);
		}
	}
	for (auto k : removedSkill)
	{
		RemoveState(k);
	}
}

void AGameBattler::Recover()
{
	hp = maxhp();
	mp = maxmp();
}


void AGameBattler::SkillEffect(AGameBattler* User, USkill* skill)
{
	//播放受击动画
	Fconfig_skill* skillData = skill->GetData();
	if (skillData != NULL)
	{
		Fconfig_effect* effect = UMyGameSingleton::Get().FindEffect(skill->id, User->race);
		if (effect != NULL)
		{
			GetMesh()->GetAnimInstance()->Montage_Play(effect->hit_anim);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), effect->hit_fx, GetTransform().GetLocation());
		}
	}

	skill->ReceiveSkillEffect(this,User);

	mp = FMath::Clamp(mp, 0, maxmp());
	hp = FMath::Clamp(hp, 0, maxhp());
}
void AGameBattler::LearnSkill(FName skillId)
{
	Fconfig_skill* skill = UMyGameSingleton::Get().FindSkill(skillId);
	if (skill == NULL)
	{
		TRACE("无效的技能ID %s", *skillId.ToString());
		return;
	}

	if (skills.Contains(skillId))
	{
		TRACE("已经学过此技能 %s", *skillId.ToString());
		return;
	}

	skill->icon.LoadSynchronous();

	USkill* s = NULL;
	
	if (skill->prefab != NULL)
	{
		s = NewObject<USkill>(this, skill->prefab);
	}
	else
	{
		s = NewObject<USkill>();
	}
	
	s->id = skillId;
	s->level = 1;

	skills.Add(skillId, s);
}

bool AGameBattler::can_move()
{
	for (auto kv : states)
	{
		if (kv.Value->GetData()->cant_move)
			return false;
	}
	return true;
}
bool AGameBattler::can_use_skill()
{
	if (skill_common_cd>0)
		return false;
	for (auto kv : states)
	{
		if (kv.Value->GetData()->cant_use_skill)
			return false;
	}
	return true;
}
bool AGameBattler::can_use_skill_target(FName skillId)
{
	if (!can_use_skill())
		return false;
	if (!skills.Contains(skillId))
		return false;

	USkill* skill = skills[skillId];
	if (skill->cd > 0 || mp < skill->GetData()->cost_mp)
		return false;

	return skill->GetData()->distance >= FVector::Dist(Target->GetTransform().GetLocation(), GetTransform().GetLocation());
}
void AGameBattler::Attack(FName skillId)
{
	if (Target == NULL)
		return;
	if (!can_use_skill_target(skillId))
		return;

	
	current_skill = skills[skillId];
	current_skill->cd = current_skill->GetData()->cd;
	skill_common_cd = current_skill->GetData()->common_cd;
	mp -= current_skill->GetData()->cost_mp;
	mp = FMath::Clamp(mp, 0, maxmp());

	Fconfig_effect* effect = UMyGameSingleton::Get().FindEffect(current_skill->id, race);
	if (effect != NULL)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(effect->start_self_anim);
	}
}

void AGameBattler::SelectTarget(AGameBattler* User)
{
	Target = User;
}


void AGameBattler::AnimNofity_SkillEffect()
{
	if (current_skill == NULL)
		return;
	TArray<AGameBattler*> targets = current_skill->ReceiveSkillGetTargets(this);

	for (auto battler : targets)
	{
		battler->SkillEffect(this, current_skill);
	}
}


void AGameBattler::AnimNofity_Shoot()
{
	if (current_skill == NULL)
		return;
	if (Target == NULL)
		return;

	Fconfig_effect* effect = UMyGameSingleton::Get().FindEffect(current_skill->id, race);
	if (effect != NULL)
	{
		//投掷体
		if (effect->fly_body != nullptr)
		{
			if (effect->fly_all_target)
			{
				TArray<AGameBattler*> targets = current_skill->ReceiveSkillGetTargets(this);

				for (auto battler : targets)
				{
					AActor* Projectile = GetWorld()->SpawnActor<AActor>(effect->fly_body, GetTransform());
					if (Projectile)
					{
						UProjectile* pj = NewObject<UProjectile>(Projectile);
						pj->RegisterComponent();
						pj->InitCreate(this, battler, current_skill);
					}
				}
			}
			else
			{
				AActor* Projectile = GetWorld()->SpawnActor<AActor>(effect->fly_body, GetTransform());
				if (Projectile)
				{
					UProjectile* pj = NewObject<UProjectile>(Projectile);
					pj->RegisterComponent();
					pj->InitCreate(this, Target, current_skill);
				}
			}
		}
	}
}

void AGameBattler::AddState(FName stateId)
{
	Fconfig_state* state = UMyGameSingleton::Get().FindState(stateId);
	if (state == NULL)
	{
		TRACE("无效的状态ID %s", *stateId.ToString());
		return;
	}

	if (states.Contains(stateId))
	{
		states[stateId]->cd = states[stateId]->GetData()->time;
		return;
	}
	
	UState* s = NULL;
	if (state->prefab != NULL)
	{
		s = NewObject<UState>(this, state->prefab);
	}
	else
	{
		s = NewObject<UState>();
	}
	s->id = stateId;
	s->cd = s->GetData()->time;
	if (state->ps != NULL)
	{
		state->ps.ToStringReference().TryLoad();
		UParticleSystemComponent* ps = NewObject<UParticleSystemComponent>(this);
		ps->SetTemplate(state->ps.Get());
		ps->RegisterComponent();
		s->PS = ps;
		ps->AttachTo(RootComponent);
	}

	states.Add(stateId, s);
}
void AGameBattler::RemoveState(FName stateId)
{
	if (states.Contains(stateId))
	{
		UState* s = states[stateId];
		if (s->PS != NULL)
		{
			s->PS->UnregisterComponent();
			s->PS = NULL;
		}
	}
	states.Remove(stateId); 
}



int32 AGameBattler::maxhp()
{ 
	int32 base = base_maxhp(); 
	for (auto kv : states)
	{
		base = base*kv.Value->GetData()->hp_percent + kv.Value->GetData()->hp_plus;
	}

	return base;
}

int32 AGameBattler::maxmp()
{ 
	int32 base = base_maxmp();
	for (auto kv : states)
	{
		base = base + kv.Value->GetData()->mp_plus;
	}

	return base;
}

int32 AGameBattler::patk()
{
	int32 base = base_patk();
	for (auto kv : states)
	{
		base = base*kv.Value->GetData()->patk_percent + kv.Value->GetData()->patk_plus;
	}

	return base;
}

int32 AGameBattler::matk()
{
	int32 base = base_matk();
	for (auto kv : states)
	{
		base = base*kv.Value->GetData()->matk_percent + kv.Value->GetData()->matk_plus;
	}

	return base;
}

int32 AGameBattler::pdef()
{
	int32 base = base_pdef();
	for (auto kv : states)
	{
		base = base + kv.Value->GetData()->pdef_plus;
	}

	return base;
}

int32 AGameBattler::mdef()
{ 
	int32 base = base_mdef();
	for (auto kv : states)
	{
		base = base + kv.Value->GetData()->mdef_plus;
	}

	return base;
}

int32 AGameBattler::hit()
{
	int32 base = base_hit();
	for (auto kv : states)
	{
		base = base + kv.Value->GetData()->hit_plus;
	}

	return base;
}
int32 AGameBattler::eva()
{
	int32 base = base_eva();
	for (auto kv : states)
	{
		base = base + kv.Value->GetData()->eva_plus;
	}

	return base;
}