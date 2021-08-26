#pragma once

#include "GameObjectManager.h"
#include "GameObject.h"
#include "Bullet.h"

//Attachable to weapons
class BulletSpawner
{

public:
	BulletSpawner(GameObjectManager* , Bullet*);
	~BulletSpawner();

	BULLET_TYPE GetBulletType();
	void SetBullet(Bullet*); //Set the bullet prototype, will not allow wrong value parsing
	void SetBulletSpeed(float speed);
	void SpawnBullet(Vector3 pos, Vector3 vel, Vector3 normal);

private:
	GameObjectManager* gom_ref;
	Bullet* bulletPrototype;
};

