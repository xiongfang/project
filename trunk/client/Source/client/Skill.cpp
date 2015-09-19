#include "client.h"
#include "Skill.h"
#include "MyCharacter.h"

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

	
}