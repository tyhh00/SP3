#include "Bullet.h"

Bullet::Bullet(Mesh* mesh, int geoTypeID, SHAPE_TYPE collisionShape, BULLET_TYPE type, Vector3 scale, bool explosive, float explosionRadius)
	: GameObject(GameObject::GAMEOBJECT_TYPE::GO_BULLET, mesh,  collisionShape)
	, bulletType(type)
{
	this->physics->SetMovable(true);
	this->physics->SetGravity(false);
	this->physics->SetEnableCollision(false);
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

GameObject* Bullet::Clone()
{
	GameObject* go = GameObject::Clone();
	Bullet* cloned = static_cast<Bullet*>(go);
	
	//Set bullet stuff
	cloned->type = type;

	return cloned;
}


	
