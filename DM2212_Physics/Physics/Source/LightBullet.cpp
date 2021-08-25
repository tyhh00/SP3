#include "LightBullet.h"
#include "Debug.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

LightBullet::LightBullet(Vector3 scale, GameObject* attachedPlayer, float _bulletSpeed)
	: Bullet(nullptr, -1, SHAPE_TYPE::CIRCLE, BULLET_TYPE::LIGHT, scale, false, 1.f, _bulletSpeed)
	, attachedPlayer(attachedPlayer)
{
	//ANIMATED SPRITE
	mesh = MeshBuilder::GenerateQuad("Wall", Color(1, 1, 1), 2.0f);
	mesh->textureID = LoadTGA("Image/PortalTravelSprite.tga");

	aliveTimer = 0.0;
	
	//enableCollision = false;
	//physics->SetEnableUpdate(false);
}

LightBullet::~LightBullet()
{
}

void LightBullet::Init()
{
}

void LightBullet::Update(double dt)
{
	aliveTimer += dt;

	DEBUG_MSG("D");

	////Attach pos of bullet to player
	//if (attachedPlayer != nullptr)
	//{
	//	this->pos = attachedPlayer->pos;
	//	this->pos.z += 1; //Set it infront of player
	//}
}

void LightBullet::CollidedWith(GameObject* go)
{
	if (go->type != GO_ENEMY)
	{
		this->dead = true;
		go->MinusCurrHealth(5);
	}
}

GameObject* LightBullet::Clone()
{
	LightBullet* cloned = new LightBullet(this->scale, this->attachedPlayer, this->bulletSpeed);
	//GameObject::CloneValues(this, cloned);
	return cloned;
}
