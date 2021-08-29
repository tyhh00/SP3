#pragma once

#include "Ability.h"
#include "BulletSpawner.h"

class BlackHoleAbility : public Ability
{

public:
	BlackHoleAbility(BulletSpawner* spawner, Mesh* mesh = nullptr);
	~BlackHoleAbility();

	void Init();

	void Update(double dt);
	void Render();
	void Reset();
	ABILITY_TYPE GetAbilityType();
	void InitSpawner(GameObjectManager* gom, Bullet* bullet);

private:
	BulletSpawner* spawner;
	
	float screen_w, screen_h;

	float heldDownFor;

	void CursorToWorldPosition(double& theX, double& theY);
};

