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

	ReceiveSkillEffect(User, skill);

	User->mp = FMath::Clamp(User->mp, 0, User->maxmp());
	User->hp = FMath::Clamp(User->hp, 0, User->maxhp());

	mp = FMath::Clamp(mp, 0, maxmp());
	hp = FMath::Clamp(hp, 0, maxhp());
}
void AGameBattler::LearnSkill(FName skillId)
{
	if (UMyGameSingleton::Get().FindSkill(skillId) == NULL)
	{
		TRACE("无效的技能ID %s", *skillId.ToString());
		return;
	}

	if (skills.Contains(skillId))
	{
		TRACE("已经学过此技能 %s", *skillId.ToString());
		return;
	}


	USkill* s = NewObject<USkill>();
	s->id = skillId;
	s->level = 1;

	skills.Add(skillId, s);
}

bool AGameBattler::can_move()
{
	return true;
}
bool AGameBattler::can_use_skill()
{
	if (skill_common_cd>0)
		return false;

	return true;
}
bool AGameBattler::can_use_skill_target(FName skillId)
{
	if (!can_use_skill())
		return false;
	if (!skills.Contains(skillId))
		return false;

	USkill* skill = skills[skillId];
	if (skill->cd > 0)
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
	if (Target != NULL)
	{
		Target->SkillEffect(this, current_skill);
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