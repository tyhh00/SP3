#pragma once

#include "Ability.h"
#include "BulletSpawner.h"

class BlackHoleAbility : public Ability
{

public:
	BlackHoleAbility();
	~BlackHoleAbility();

	void Init(GameObject* player, BulletSpawner* spawner);

	void Update(double dt);
	void Render();
	ABILITY_TYPE GetAbilityType();

private:
	BulletSpawner* spawner;
	GameObject* player;

};

