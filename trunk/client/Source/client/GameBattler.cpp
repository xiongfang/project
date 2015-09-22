#include "client.h"
#include "GameBattler.h"
#include "Skill.h"


void AGameBattler::SkillEffect(AGameBattler* User, USkill* skill)
{

	ReceiveSkillEffect(User, skill);

	User->mp = FMath::Clamp(User->mp, 0, User->maxmp());
	User->hp = FMath::Clamp(User->hp, 0, User->maxhp());

	mp = FMath::Clamp(mp, 0, maxmp());
	hp = FMath::Clamp(hp, 0, maxhp());
}