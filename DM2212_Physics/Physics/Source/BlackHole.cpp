#include "BlackHole.h"

BlackHoleAbility::BlackHoleAbility()
	: Ability('Z', ABILITY_TYPE::ABILITY_BLACKHOLE, 5.0)
{
}

BlackHoleAbility::~BlackHoleAbility()
{
}

void BlackHoleAbility::Init(GameObject* player, BulletSpawner* spawner)
{
	this->player = player;
	this->spawner = spawner;
}

void BlackHoleAbility::Update(double dt)
{
}

void BlackHoleAbility::Render()
{
}

ABILITY_TYPE BlackHoleAbility::GetAbilityType()
{
	return ABILITY_TYPE();
}
