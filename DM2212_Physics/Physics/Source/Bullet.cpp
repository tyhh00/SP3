#include "Bullet.h"

Bullet::Bullet(Mesh* mesh, int geoTypeID, SHAPE_TYPE collisionShape, BULLET_TYPE type, Vector3 scale, bool explosive, float explosionRadius)
	: GameObject(GameObject::GAMEOBJECT_TYPE::GO_BULLET, mesh,  collisionShape)
	, type(type)
{
	this->explosive = explosive;
	this->explosiveRadius = explosionRadius;
	this->scale = scale;
}

Bullet::~Bullet()
{
	//Need not delete mesh since its handled in Scenes themselves
}


	
