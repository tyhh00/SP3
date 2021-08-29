#include "BananaBullet.h"
#include "Debug.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

BananaBullet::BananaBullet(Vector3 scale, GameObject* attachedPlayer, float _bulletSpeed)
	: Bullet(nullptr, -1, SHAPE_TYPE::CIRCLE, BULLET_TYPE::BANANA, scale, false, 1.f, _bulletSpeed)
	, attachedPlayer(attachedPlayer)
{
	//ANIMATED SPRITE
	mesh = MeshBuilder::GenerateQuad("Wall", Color(1, 1, 1), 2.0f);
	mesh->textureID = LoadTGA("Image/Tiles/GEO_JUNGLE_BANANA.tga");

	aliveTimer = 0.0;
	
	//enableCollision = false;
	//physics->SetEnableUpdate(false);
}

BananaBullet::~BananaBullet()
{
}

void BananaBullet::Init()
{
}

void BananaBullet::Update(double dt)
{
	aliveTimer += dt;

	if (aliveTimer > 10)
		this->dead = true;
}

void BananaBullet::CollidedWith(GameObject* go)
{
	if (go->type == GO_PLAYER)
	{
		this->dead = true;
		go->MinusCurrHealth(10);
	}
}

GameObject* BananaBullet::Clone()
{
	BananaBullet* cloned = new BananaBullet(this->scale, this->attachedPlayer, this->bulletSpeed);
	//GameObject::CloneValues(this, cloned);
	return cloned;
}
