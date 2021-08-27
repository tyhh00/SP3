#pragma once

#include "GameObjectManager.h"
#include "GameObject.h"
#include "Bullet.h"

//Attachable to weapons
class BulletSpawner
{

public:
	BulletSpawner();
	BulletSpawner(GameObjectManager* , Bullet*);
	~BulletSpawner();

	BULLET_TYPE GetBulletType();
	void SetGOMRef(GameObjectManager* gom);
	void SetBullet(Bullet*); //Set the bullet prototype, will not allow wrong value parsing
	void SetBulletSpeed(float speed);
	void SetBulletStatus(BULLET_STATUS status);
	void SpawnBullet(Vector3 pos, Vector3 vel, Vector3 normal);

private:
	GameObjectManager* gom_ref;
	Bullet* bulletPrototype;
	std::vector<GameObject*> activeBullets;
};

