#include "PlasmaBullet.h"
#include "Debug.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

PlasmaBullet::PlasmaBullet(Vector3 scale, GameObject* attachedPlayer)
//Init with nullptr mesh and -1 geoType since we're overriding mesh with animatedSprite
	: Bullet(nullptr, -1, SHAPE_TYPE::CIRCLE, BULLET_TYPE::PLASMA, scale, true, 5.5f)
	, attachedPlayer(attachedPlayer)
{
	//ANIMATED SPRITE
	animatedSprite = MeshBuilder::GenerateSpriteAnimation(4, 4, 2.0f, 2.0f);
	animatedSprite->AddAnimation("chargingup", 0, 4);
	animatedSprite->AddAnimation("fullycharged", 5, 12);
	animatedSprite->AddAnimation("chargingdown", 13, 15);
	mesh = animatedSprite;
	mesh->textureID = LoadTGA("Image/robot_plasma.tga");

	animatedSprite->PlayAnimation("chargingup", 0, 1.0f);

	aliveTimer = 0.0;

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
	animatedSprite->Update(dt);

	DEBUG_MSG(&animatedSprite << ": " << animatedSprite->currentFrame);

	//Attach pos of bullet to player
	if (attachedPlayer != nullptr)
	{
		this->pos = attachedPlayer->pos;
		this->pos.z += 1; //Set it infront of player
	}

	if (aliveTimer > 1.0 && state == CHARGINGUP_PHASE1)
	{
		animatedSprite->PlayAnimation("fullycharged", -1, 3.0);
		state = CHARGINGUP_PHASE2;
	}
	else if (aliveTimer > 2.0 && state == CHARGINGUP_PHASE2)
	{
		attachedPlayer = nullptr; //Disable attaching of pos to player

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
	go->dead = true;
}

GameObject* PlasmaBullet::Clone()
{
	PlasmaBullet* cloned = new PlasmaBullet(this->scale, this->attachedPlayer);
	//GameObject::CloneValues(this, cloned);
	return cloned;
}
