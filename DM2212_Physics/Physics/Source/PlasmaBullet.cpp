#include "PlasmaBullet.h"
#include "Debug.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

PlasmaBullet::PlasmaBullet(Mesh* mesh, int geoTypeID, Vector3 scale)
	: Bullet(mesh, geoTypeID, SHAPE_TYPE::CIRCLE, BULLET_TYPE::PLASMA, scale, explo, rad)
{
	//ANIMATED SPRITE
	animatedSprite = MeshBuilder::GenerateSpriteAnimation(4, 4, 2.0f, 2.0f);
	animatedSprite->AddAnimation("chargingup", 0, 4);
	animatedSprite->AddAnimation("fullycharged", 5, 12);
	animatedSprite->AddAnimation("chargingdown", 13, 15);
	mesh = animatedSprite;
	mesh->textureID = LoadTGA("Image/robot_plasma.tga");

	animatedSprite->PlayAnimation("chargingup", 0, 1.0f);

	//PLASMA BULLET FUNCTIONALITY
	state = PLASMABULLET_STATE::CHARGINGUP_PHASE1;
	enableCollision = false;
	physics->SetEnableUpdate(false);
}

PlasmaBullet::~PlasmaBullet()
{
	delete animatedSprite;
}

void PlasmaBullet::Init()
{
}

void PlasmaBullet::Update(double dt)
{
	aliveTimer += dt;
	if (aliveTimer > 1.0 && state == CHARGINGUP_PHASE1)
	{
		animatedSprite->PlayAnimation("fullycharged", -1, 3.0);
		state = CHARGINGUP_PHASE2;
	}
	else if (aliveTimer > 3.0 && state == CHARGINGUP_PHASE2)
	{
		state = FULLYCHARGED;
		enableCollision = true;
		physics->SetEnableUpdate(true);
	}
	else if (aliveTimer > 9.0 && state == FULLYCHARGED)
	{
		state = CHARGINGDOWN;
		animatedSprite->PlayAnimation("chargingdown", 0, 0.5f);
	}
	else if (aliveTimer > 9.5 && state == CHARGINGDOWN)
	{
		dead = true;
		enableCollision = false;
		physics->SetEnableUpdate(false);
	}
}

void PlasmaBullet::CollidedWith(GameObject* go)
{
	DEBUG_MSG("Plasma Bullet Collided with: " << go->geoTypeID);
	dead = true;
}

PlasmaBullet* PlasmaBullet::Clone()
{
	GameObject* go = GameObject::Clone();
	PlasmaBullet* cloned = dynamic_cast<PlasmaBullet*>(go);
	cloned->animatedSprite = this->animatedSprite->Clone();
}
