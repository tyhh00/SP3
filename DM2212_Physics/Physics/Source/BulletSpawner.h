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

	BULLET_TYPE getBulletType();
	void SetBullet(Bullet*); //Set the bullet prototype, will not allow wrong value parsing

	void SpawnBullet(Vector3 vel, Vector3 dir); // Creates a bullet GameObject and adds it into the defined GOManager

private:
	GameObjectManager* gom_ref;
};

