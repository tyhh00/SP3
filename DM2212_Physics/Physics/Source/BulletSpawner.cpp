#include "BulletSpawner.h"
#include "Debug.h"
BulletSpawner::BulletSpawner()
{
}
BulletSpawner::BulletSpawner(GameObjectManager* sceneGOM, Bullet* bulletPrototype)
	: gom_ref(sceneGOM)
	, bulletPrototype(bulletPrototype)
{}

BulletSpawner::~BulletSpawner()
{
	delete bulletPrototype; //Since this is not added into GOManagement, we settle it here
	bulletPrototype = nullptr; 

	for (auto& go : activeBullets)
	{
		if (go)
		{
			delete go;
			go = nullptr;
		}
	}
}

BULLET_TYPE BulletSpawner::GetBulletType()
{
	return bulletPrototype->GetBulletType();
}

void BulletSpawner::SetGOMRef(GameObjectManager* gom)
{
	gom_ref = gom;
}

void BulletSpawner::SetBullet(Bullet* bullet)
{
	if (bulletPrototype)
		delete bulletPrototype;
	bulletPrototype = bullet;
}

void BulletSpawner::SetBulletSpeed(float speed)
{
	bulletPrototype->SetBulletSpeed(speed);
}

void BulletSpawner::SpawnBullet(Vector3 pos, Vector3 vel, Vector3 normal)
{
	GameObject* clone = bulletPrototype->Clone();
	clone->pos = pos;
	clone->physics->pos = pos;
	if (vel.IsZero()) vel = Vector3(1, 0, 0);
	clone->physics->SetVelocity(vel.Normalize() * bulletPrototype->GetBulletSpeed());
	clone->physics->SetNormal(normal);
	gom_ref->AddGO(clone);
	activeBullets.push_back(clone);

	for (auto& go : activeBullets)
	{
		if (!go)
			go = nullptr;
	}
	activeBullets.erase(std::remove(activeBullets.begin(), activeBullets.end(), nullptr),
		activeBullets.end());
}
