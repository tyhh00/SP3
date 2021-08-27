#pragma once

#include "GameObject.h"
#include "BulletSpawner.h"

enum PLASMAENGINE_TYPE
{
	FRONT,
	FRONT_BACK,
	FRONT_BACK_LEFT_RIGHT
};

class PlasmaEngine : public GameObject

{

public:
	PlasmaEngine();
	~PlasmaEngine();
	void Init(BulletSpawner* spawner, GameObject* player, float bulletSpawnCD = 3.0);
	void Update(double dt);
private:
	float lastSpawnBullet;
	float bulletSpawnCD;

	float rangeToAttack;

	GameObject* player;

	PLASMAENGINE_TYPE engineType;
	BulletSpawner* spawner;
};

