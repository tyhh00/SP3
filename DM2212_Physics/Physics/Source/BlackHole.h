#pragma once

#include "Ability.h"
#include "BulletSpawner.h"

class BlackHoleAbility : public Ability
{

public:
	BlackHoleAbility(GameObject* player, BulletSpawner* spawner, Camera* cam, float screen_w, float screen_h, Mesh* mesh = nullptr);
	~BlackHoleAbility();

	void Init();

	void Update(double dt);
	void Render();
	ABILITY_TYPE GetAbilityType();

private:
	BulletSpawner* spawner;
	GameObject* player;
	float screen_w, screen_h;

	float heldDownFor;

	void CursorToWorldPosition(double& theX, double& theY);
};

