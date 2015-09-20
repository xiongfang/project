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


UProjectile::UProjectile()
{
	skill = NULL;
	Owner = NULL;
	Target = NULL;
	PrimaryComponentTick.bCanEverTick = true;
}

void UProjectile::InitCreate(AMyCharacter* o, AMyCharacter* t, USkill* s)
{
	check(o);
	check(t);
	check(s);

	Owner = o;
	Target = t;
	skill = s;
	Fconfig_effect* effect = UMyGameSingleton::Get().FindEffect(s->id, Owner->race);
	speed = effect->fly_speed;

	//初始化偏移
	if (Target != NULL)
	{
		TargetPosition = Target->GetTransform().GetLocation();
	}
	FVector current = GetOwner()->GetTransform().GetLocation();
	FRotator rot= FRotationMatrix::MakeFromX(TargetPosition - current).Rotator();
	GetOwner()->AddActorWorldOffset(rot.RotateVector(effect->fly_offset));
	//初始化方向
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
	FVector dir = TargetPosition - current;
	dir.Normalize();

	GetOwner()->AddActorWorldOffset(dir*speed*DeltaTime);
	current = GetOwner()->GetTransform().GetLocation();

	GetOwner()->SetActorRotation(FRotationMatrix::MakeFromX(TargetPosition - current).Rotator());

	//判定移动完成
	if (FVector::DistSquared(current, TargetPosition) < 30.0f)
	{
		if (Target != NULL)
		{
			Target->SkillEffect(Owner, skill);
		}
		Fconfig_effect* effect = UMyGameSingleton::Get().FindEffect(skill->id, Owner->race);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), effect->fly_explosion_fx, TargetPosition);
		//自动销毁自身
		GetOwner()->Destroy();
	}
}