#include "PlasmaBullet.h"
#include "Debug.h"

PlasmaBullet::PlasmaBullet(Mesh* mesh, int geoTypeID, Vector3 scale)
	: Bullet(mesh, geoTypeID, SHAPE_TYPE::CIRCLE, BULLET_TYPE::PLASMA, scale, explo, rad)
{
}

PlasmaBullet::~PlasmaBullet()
{
	delete animatedSpirte;
}

void PlasmaBullet::Init()
{
	//Init animated sprite
}

void PlasmaBullet::Update(double dt)
{

}

void PlasmaBullet::CollidedWith(GameObject* go)
{
	DEBUG_MSG("Plasma Bullet Collided with: " << go->geoTypeID);
}

PlasmaBullet* PlasmaBullet::Clone()
{
	GameObject* go = GameObject::Clone();
	PlasmaBullet* cloned = dynamic_cast<PlasmaBullet*>(go);
	cloned->animatedSpirte = this->animatedSpirte;
}
