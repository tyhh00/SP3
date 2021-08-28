#pragma once
#include "Mesh.h"
#include "GameObject.h"
#include <vector>

enum BULLET_TYPE
{
	PLASMA,
	LIGHT,
	BANANA,
	BLACKHOLE,
	BULLET_COUNT
};

enum BULLET_STATUS
{
	ALLY,
	ENEMY
};

//To be used by Bullet Spawner to create actual bullet
class Bullet : public GameObject
{
public:
	~Bullet();

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;

	BULLET_TYPE GetBulletType();

	//GO Virtual Functions
	virtual void CollidedWith(GameObject*) = 0;
	virtual GameObject* Clone() = 0;

	void SetBulletSpeed(float _bulletSpeed);
	float GetBulletSpeed();

	void SetExplosionRadius(float rad);

	void SetBulletStatus(BULLET_STATUS status);

protected:
	Bullet(Mesh* mesh, int geoTypeID, SHAPE_TYPE collisionShape, BULLET_TYPE type, Vector3 scale, bool explosive, float explosionRadius, float _bulletSpeed);
	float bulletSpeed;
private:
	BULLET_TYPE bulletType;
	BULLET_STATUS bulletStatus;
	
};

