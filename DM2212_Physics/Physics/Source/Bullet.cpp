#include "Bullet.h"

void Bullet::SetBulletSpeed(float _bulletSpeed)
{
	bulletSpeed = _bulletSpeed;
}

float Bullet::GetBulletSpeed()
{
	return bulletSpeed;
}

void Bullet::SetExplosionRadius(float rad)
{
	this->explosiveRadius = rad;
}

Bullet::Bullet(Mesh* mesh, int geoTypeID, SHAPE_TYPE collisionShape, BULLET_TYPE type, Vector3 scale, bool explosive, float explosionRadius, float _bulletSpeed)
	: GameObject(GameObject::GAMEOBJECT_TYPE::GO_BULLET, mesh, geoTypeID, collisionShape)
	, bulletType(type)
	, bulletSpeed(_bulletSpeed)
	, bulletStatus(ALLY)
{
	this->AddToGOCollisionWhitelist(GO_BULLET);
	this->physics->SetMovable(true);
	this->physics->SetGravity(false);
	this->physics->SetEnableCollisionResponse(false);
	this->explosive = explosive;
	this->explosiveRadius = explosionRadius;
	this->scale = scale;
}

Bullet::~Bullet()
{
	//Need not delete mesh since its handled in Scenes themselves
}

BULLET_TYPE Bullet::GetBulletType()
{
	return bulletType;
}


	
