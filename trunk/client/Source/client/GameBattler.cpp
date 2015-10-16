#include "client.h"
#include "GameBattler.h"
#include "Skill.h"
#include "config.h"

FName DeadStateName("战斗不能");


AGameBattler::AGameBattler()
{
	timer_combat_cd = 10.0f;
}

void AGameBattler::TriggerEnterCombating()
{
	timer_combat_cd = UMyGameSingleton::Get().combat_cd;
	if (!combating)
	{
		combating = true;
		NotifyEnterCombating();
	}
}
void AGameBattler::CheckLeaveCombating(float DeltaTime)
{
	if (combating)
	{
		if (Target == NULL || !Target->IsEnemy(this) || (Target->IsDead() && Target->IsEnemy(this)))
		{
			timer_combat_cd -= DeltaTime;
			if (timer_combat_cd < 0)
			{
				combating = false;
				NotifyLeaveCombating();
			}
		}
	}
}
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

	CheckLeaveCombating(DeltaTime);
}

int32 AGameBattler::damage(AGameBattler* User,DamageFlag dt, int32 hp_damage, int32 mp_damage)
{
	combating = true;
	timer_combat_cd = UMyGameSingleton::Get().combat_cd;

	bool dead = IsDead();

	if (dt != DamageFlag::Miss)
	{
		hp -= hp_damage;
		mp -= mp_damage;

		hp = FMath::Clamp(hp, 0, maxhp());
		mp = FMath::Clamp(mp, 0, maxmp());
		if (hp_damage >= 0)
			ShowDamageUI(FString::Printf(TEXT("%d"), FMath::Abs(hp_damage)), FColor::Red);
		else
			ShowDamageUI(FString::Printf(TEXT("+%d"), FMath::Abs(hp_damage)), FColor::Green);

	}
	else
	{
		ShowDamageUI(TEXT("MISS"), FColor::White);
	}


	if (!dead && IsDead())
	{
		TMap<FName, UState*> s = states;
		for (auto state : s)
		{
			this->RemoveState(state.Key);
		}
		AddState(DeadStateName);

		Event_OnDead(User);
	}
	return hp;
}
void AGameBattler::Recover()
{
	hp = maxhp();
	mp = maxmp();

	TMap<FName, UState*> s = states;
	for (auto state : s)
	{
		this->RemoveState(state.Key);
	}
	if (this->GetCapsuleComponent() != NULL)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	//if (GetMesh() != NULL)
	//{
	//	GetMesh()->PutAllRigidBodiesToSleep();
	//	GetMesh()->SetAllBodiesSimulatePhysics(false);
	//}
}


void AGameBattler::SkillEffect(AGameBattler* User, USkill* skill)
{
	bool dead = IsDead();

	skill->SkillEffect(this,User);

	mp = FMath::Clamp(mp, 0, maxmp());
	hp = FMath::Clamp(hp, 0, maxhp());


	if (!dead && IsDead())
	{
		TMap<FName, UState*> s = states;
		for (auto state : s)
		{
			this->RemoveState(state.Key);
		}
		AddState(DeadStateName);

		Event_OnDead(User);
	}

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
	if (IsDead())
		return false; 
	for (auto kv : states)
	{
		if (kv.Value->GetData()->cant_move)
			return false;
	}
	return true;
}

bool AGameBattler::can_use_skill(USkill* skill)
{
	if (skill_common_cd>0)
		return false;
	for (auto kv : states)
	{
		if (kv.Value->GetData()->cant_use_skill)
			return false;
	}
	if (!skills.Contains(skill->id))
		return false;
	if (skill->cd > 0 || mp < skill->GetData()->cost_mp)
		return false;
	return true;
}
bool AGameBattler::can_use_skill_target(USkill* skill, AGameBattler* bt)
{
	check(skill);
	check(bt);
	if (!can_use_skill(skill))
		return false;
	if (!skill->valid_target(this,bt))
		return false;

	return skill->in_distance(FVector::Dist(bt->GetTransform().GetLocation(), GetTransform().GetLocation()));
}
bool AGameBattler::Attack(FName skillId)
{
	combating = true;
	timer_combat_cd = UMyGameSingleton::Get().combat_cd;

	if (Target == NULL)
		return false;
	if (!skills.Contains(skillId))
		return false;

	USkill* skill = skills[skillId];

	if (!can_use_skill_target(skill, Target))
		return false;

	current_skill = skill;
	current_skill->cd = current_skill->GetData()->cd;
	skill_common_cd = current_skill->GetData()->common_cd;
	mp -= current_skill->GetData()->cost_mp;
	mp = FMath::Clamp(mp, 0, maxmp());

	Fconfig_effect* effect = UMyGameSingleton::Get().FindEffect(current_skill->id, race());
	if (effect != NULL)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(effect->start_self_anim);
	}
	else
	{
		Target->SkillEffect(this, skill);
	}

	return true;
}

void AGameBattler::SelectTarget(AGameBattler* User)
{
	Target = User;
	TriggerEnterCombating();
}


void AGameBattler::AnimNofity_SkillEffect()
{
	if (current_skill == NULL)
		return;
	TArray<AGameBattler*> targets = current_skill->GetTargets(this);

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

	Fconfig_effect* effect = UMyGameSingleton::Get().FindEffect(current_skill->id, race());
	if (effect != NULL)
	{
		//投掷体
		if (effect->fly_body != nullptr)
		{
			if (effect->fly_all_target)
			{
				TArray<AGameBattler*> targets = current_skill->GetTargets(this);

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

	s->Receive_OnAdd(this);
	states.Add(stateId, s);
}
void AGameBattler::RemoveState(FName stateId)
{
	if (states.Contains(stateId))
	{
		UState* s = states[stateId];
		s->Receive_OnRemove(this);
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
		base = base*(1.0f+kv.Value->GetData()->hp_percent) + kv.Value->GetData()->hp_plus;
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
		base = base*(1.0f + kv.Value->GetData()->patk_percent) + kv.Value->GetData()->patk_plus;
	}

	return base;
}

int32 AGameBattler::matk()
{
	int32 base = base_matk();
	for (auto kv : states)
	{
		base = base*(1.0f + kv.Value->GetData()->matk_percent) + kv.Value->GetData()->matk_plus;
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


TArray<USkill*> AGameBattler::GetSkills()
{
	TArray<USkill*> data;
	for (auto kv : skills)
	{
		data.Add(kv.Value);
	}
	return data;
}


void AGameBattler::Event_OnDead_Implementation(AGameBattler* User)
{
	USoundBase* ds = dead_sound();
	if(ds != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ds, GetActorLocation());
	}
	if (this->GetCapsuleComponent() != NULL)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//if (GetMesh() != NULL)
	//{
	//	GetMesh()->SetAllBodiesSimulatePhysics(true);
	//	GetMesh()->WakeAllRigidBodies();
	//}
}

class BattlerSort
{
public:
	FVector origin;

	bool operator()(AGameBattler& A, AGameBattler& B) const
	{
		return FVector::Dist(origin, A.GetActorLocation()) < FVector::Dist(origin, B.GetActorLocation());
	}
};
TArray<AGameBattler*> AGameBattler::FindBattlers(float Radius)
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

	UWorld* World = GetWorld();
	bool const bHit = World->OverlapMultiByObjectType(OutOverlaps, GetActorLocation(), FQuat::Identity, ObjectParams, FCollisionShape::MakeSphere(Radius), Params);
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

	BattlerSort sort;
	sort.origin = GetActorLocation();
	results.Sort(sort);

	return results;
}

void AGameBattler::SerializeProperty(FArchive& ar)
{
	ar << hp << mp << (uint8&)(camp) << immortal << combating<< skills << states;
}
