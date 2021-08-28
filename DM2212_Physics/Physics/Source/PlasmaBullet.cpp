#include "PlasmaBullet.h"
#include "Debug.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

PlasmaBullet::PlasmaBullet(Vector3 scale, GameObject* attachedPlayer, float bulletSpeed)
//Init with nullptr mesh and -1 geoType since we're overriding mesh with animatedSprite
	: Bullet(nullptr, -1, SHAPE_TYPE::CIRCLE, BULLET_TYPE::PLASMA, scale, true, 5.5f, bulletSpeed)
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

	//Attach pos of bullet to player
	if (attachedPlayer != nullptr)
	{
		this->pos = attachedPlayer->pos;
		this->pos.z += 1; //Set it infront of player
	}

	if (aliveTimer > 0.5 && state == CHARGINGUP_PHASE1)
	{
		animatedSprite->Reset();
		animatedSprite->PlayAnimation("fullycharged", -1, 3.0);
		state = CHARGINGUP_PHASE2;
	}
	else if (aliveTimer > 0.55 && state == CHARGINGUP_PHASE2)
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
	if (go->type == GameObject::GO_TILE && CanCollideWith(go->type) && go->CanCollideWith(this->type))
	{
		if (go->IsResponable())
		{
			go->SetTemporaryDisable(4);
		}
		else if (go->IsDamagableByExplosive())
		{
			go->dead = true;
		}
		this->dead = true;
	}
	else if (go->type == GameObject::GO_PLAYER && CanCollideWith(go->type) && go->CanCollideWith(this->type))
	{
		Vector3 dist = go->pos - this->pos;
		float distSquare = dist.LengthSquared();
		if (distSquare <= AOESpread * AOESpread)
		{
			float multi = ((AOESpread * AOESpread) - distSquare) / (AOESpread * AOESpread);
			if (multi < 0.15) multi = 0.15;
			go->currentHP -= multi * max_damage;
		}
		this->dead = true;
	}
}

GameObject* PlasmaBullet::Clone()
{
	PlasmaBullet* cloned = new PlasmaBullet(this->scale, this->attachedPlayer);
	//GameObject::CloneValues(this, cloned);
	return cloned;
}
