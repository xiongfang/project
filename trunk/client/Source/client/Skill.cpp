#include "client.h"
#include "Skill.h"
#include "MyCharacter.h"
#include "config.h"

Fconfig_skill* USkill::GetData()
{
	return UMyGameSingleton::Get().FindSkill(id);
}


void USkill::Update(float deltaTime)
{
	if (cd > 0)
	{
		cd -= deltaTime;
		cd = FMath::Max(cd, 0.0f);
	}
}

void TraceTargetRange(AGameBattler* User, FVector TargetPos,Fconfig_skill* skillData, TArray<AGameBattler*>& result)
{
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
		return;
	}
	float Radius = skillData->range;
	UWorld* World = User->GetWorld();
	bool const bHit = World->OverlapMultiByObjectType(OutOverlaps, TargetPos, FQuat::Identity, ObjectParams, FCollisionShape::MakeSphere(Radius), Params);
	if (bHit)
	{
		for (auto hit : OutOverlaps)
		{
			AActor* hitActor = hit.Actor.Get();
			AGameBattler* battler = Cast<AGameBattler>(hitActor);
			if (battler != NULL)
			{
				if (skillData->target_type == SkillEffectTargetType::Self && battler == User)
				{
					result.Add(battler);
				}
				else if (skillData->target_type == SkillEffectTargetType::Enemy && User->IsEnemy(battler))
				{
					result.Add(battler);
				}
				else if (skillData->target_type == SkillEffectTargetType::Friend && !User->IsEnemy(battler))
				{
					result.Add(battler);
				}
			}
		}
	}
}
TArray<AGameBattler*> USkill::GetTargets_Implementation(AGameBattler* User)
{
	TArray<AGameBattler*> targets;
	Fconfig_skill* skillData = GetData();
	if (skillData->range_type == SkillRangeType::None)
	{
		if (User->Target != NULL)
			targets.Add(User->Target);
	}
	else if (skillData->range_type == SkillRangeType::Self)
	{
		TraceTargetRange(User, User->GetActorLocation(), skillData, targets);
	}
	else if (skillData->range_type == SkillRangeType::Target && User->Target!=NULL)
	{
		TraceTargetRange(User, User->Target->GetActorLocation(), skillData, targets);
	}

	return targets;
}
void USkill::SkillEffect_Implementation(AGameBattler* Target, AGameBattler* User)
{
	//�����ж�
	Fconfig_skill* skillData = GetData();

	bool hit = skillData->must_hit || (User->hit() - Target->eva() >= FMath::Rand() % 10000);

	if (hit)
	{
		int atk = skillData->damage_type == SkillDamageType::Physic ? User->patk(): User->matk();
		atk = atk*skillData->atk_percent + skillData->atk_plus;
		int def = skillData->damage_type == SkillDamageType::Physic ? Target->pdef(): Target->mdef();
		int dmg = skillData->ignoring_defense ? atk:(atk * (1.0f-(def / (5000.0f + def))));
		Target->hp -= dmg;
		if (dmg>=0)
			Target->ShowDamageUI(FString::Printf(TEXT("%d"),FMath::Abs(dmg)),FColor::Red);
		else
			Target->ShowDamageUI(FString::Printf(TEXT("+%d"), FMath::Abs(dmg)), FColor::Green);

		//����buff
		for (auto buff : skillData->state_plus)
		{
			if (buff.rate>FMath::FRand())
				Target->AddState(buff.Name);
		}
		//�Ƴ�buff
		for (auto buff : skillData->state_minus)
		{
			if (buff.rate>FMath::FRand())
				Target->RemoveState(buff.Name);
		}

		//�����ܻ�����
		Fconfig_effect* effect = UMyGameSingleton::Get().FindEffect(id, User->race);
		if (effect != NULL)
		{
			Target->GetMesh()->GetAnimInstance()->Montage_Play(effect->hit_anim);
		}

	}
	else
	{
		Target->ShowDamageUI(TEXT("δ����"));
	}

	//�����ܻ���Ч,��Ч
	Fconfig_effect* effect = UMyGameSingleton::Get().FindEffect(id, User->race);
	if (effect != NULL)
	{
		UGameplayStatics::SpawnEmitterAtLocation(Target->GetWorld(), effect->hit_fx, Target->GetTransform().GetLocation());
		UGameplayStatics::PlaySoundAtLocation(Target->GetWorld(), effect->hit_sound, Target->GetTransform().GetLocation());
	}
	//֪ͨĿ���ܻ�
	Target->Event_OnHit(User, this);
}

UProjectile::UProjectile()
{
	skill = NULL;
	Owner = NULL;
	Target = NULL;
	PrimaryComponentTick.bCanEverTick = true;
}

void UProjectile::InitCreate(AGameBattler* o, AGameBattler* t, USkill* s)
{
	check(o);
	check(t);
	check(s);

	Owner = o;
	Target = t;
	skill = s;
	Fconfig_effect* effect = UMyGameSingleton::Get().FindEffect(s->id, Owner->race);
	speed = effect->fly_speed;

	//��ʼ��ƫ��
	if (Target != NULL)
	{
		TargetPosition = Target->GetTransform().GetLocation();
	}
	FVector current = GetOwner()->GetTransform().GetLocation();
	FRotator rot= FRotationMatrix::MakeFromX(TargetPosition - current).Rotator();
	GetOwner()->AddActorWorldOffset(rot.RotateVector(effect->fly_offset));
	//��ʼ������
	GetOwner()->SetActorRotation(rot);
}
void UProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void UProjectile::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Target != NULL)
	{
		TargetPosition = Target->GetTransform().GetLocation();
	}
	FVector current = GetOwner()->GetTransform().GetLocation();
	FVector dist = TargetPosition - current;
	FVector dir = dist;
	dir.Normalize();
	dir = dir*speed*DeltaTime;
	if (dir.SizeSquared() > dist.SizeSquared()) //��ֹ������Χ
		dir = dist;

	GetOwner()->AddActorWorldOffset(dir);
	current = GetOwner()->GetTransform().GetLocation();

	GetOwner()->SetActorRotation(FRotationMatrix::MakeFromX(TargetPosition - current).Rotator());

	//�ж��ƶ����
	if (FVector::DistSquared(current, TargetPosition) < 30.0f)
	{
		if (Target != NULL)
		{
			Target->SkillEffect(Owner, skill);
		}
		Fconfig_effect* effect = UMyGameSingleton::Get().FindEffect(skill->id, Owner->race);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), effect->fly_explosion_fx, TargetPosition);
		UGameplayStatics::PlaySoundAtLocation(Target->GetWorld(), effect->fly_explosion_sound, TargetPosition);
		//�Զ���������
		GetOwner()->Destroy();
	}
}


Fconfig_state* UState::GetData()
{
	return UMyGameSingleton::Get().FindState(id);
}

Fconfig_task* UTask::GetData()
{
	return UMyGameSingleton::Get().FindTask(id);
}


bool UTask::can_finish_Implementation(AMyCharacter* owner)
{
	if (State != TaskState::GOING)
		return false;

	Fconfig_task* data = GetData();
	for (auto d : data->condition)
	{
		if (!owner->ItemEnough(d.Name, d.Num))
		{
			return false;
		}
	}
	return true;
}

void UTask::finish_Implementation(AMyCharacter* owner)
{
	Fconfig_task* data = GetData();
	for (auto d : data->condition)
	{
		owner->ItemLose(d.Name, d.Num);
	}
	//����
	for (auto d : data->rewards)
	{
		owner->ItemAdd(d.Name, d.Num);
	}
	State = TaskState::FINISHED;
}