#include "BlackHoleBullet.h"
#include "Debug.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

BlackHoleBullet::BlackHoleBullet(Mesh* mesh, int geoTypeID, Vector3 scale, GameObject* attachedPlayer, float bulletSpeed)
//Init with nullptr mesh and -1 geoType since we're overriding mesh with animatedSprite
	: Bullet(mesh, geoTypeID, SHAPE_TYPE::CIRCLE, BULLET_TYPE::BLACKHOLE, scale, true, 5.5f, bulletSpeed)
	, attachedPlayer(attachedPlayer)
{
	//ANIMATED SPRITE
	
	spriteRotation = 0.0;
	spriteRotationSpeed = Math::DegreeToRadian(60);

	this->maxScale = scale;
	normalisedScale = scale.Normalized();
	this->scale = Vector3(0.1, 0.1, 0.1);

	scaleSpeed = (maxScale.Length() - this->scale.Length()) / 0.5;

	aliveTimer = 0.0;

	thrownDirection = UNSET;

	//PLASMA BULLET FUNCTIONALITY
	state = BLACKHOLEBULLET_STATE::ROLLING;

	this->AddToResponseWhitelist(GO_PLAYER);

	enableCollision = true;
	physics->SetMovable(true);
	physics->SetEnableUpdate(true);
	physics->SetIsBouncable(true);
	physics->SetInelasticity(0.5f);
	physics->SetGravity(Vector3(0, -30, 0));
	physics->SetGravityUpdate(true);
	physics->SetEnableCollisionResponse(true);
}



BlackHoleBullet::~BlackHoleBullet()
{
}

void BlackHoleBullet::Init()
{
}

void BlackHoleBullet::Update(double dt)
{
	aliveTimer += dt;
	deltaTime = dt;

	if (thrownDirection == UNSET)
	{
		if (physics->GetVelocity().x > 0)
		{
			thrownDirection = RIGHT;
		}
		else
		{
			thrownDirection = LEFT;
		}
	}

	//Attach pos of bullet to player
	if (attachedPlayer != nullptr)
	{
		this->pos.z = attachedPlayer->pos.z+1; //Set it infront of player
	}

	if (aliveTimer < 0.5)
	{
		this->scale += normalisedScale * scaleSpeed * dt; 
	}

	if (aliveTimer > 1 && state == ROLLING)
	{
		if (physics->GetVelocity().LengthSquared() > 1.1)
		{
			physics->AddVelocity(Vector3(Math::Clamp(physics->GetVelocity().x, -1.0f, 1.0f) * -9 * (aliveTimer) *dt, 0, 0));
		}
	}

	//else if (aliveTimer >= 0.5 && state == ROLLING)
	//{
	//	//Thrown direction allows the BH to cling onto walls
	//	if (thrownDirection == RIGHT && physics->GetVelocity().x < 0.2
	//		||
	//		thrownDirection == LEFT && physics->GetVelocity().x > -0.8)
	//	{

	if (aliveTimer >= 0.5 && state == ROLLING)
	{
			//Thrown direction allows the BH to cling onto walls
		if (thrownDirection == RIGHT && physics->GetVelocity().x <= 1
			||
			thrownDirection == LEFT && physics->GetVelocity().x >= -1)
		{
			if (state == ROLLING) //Upon happen once set alive timer back to 0.5 so suction last for 2.5s
				aliveTimer = 0.5;
			physics->SetInelasticity(0.99f);
			physics->SetIsBouncable(false);
			physics->SetGravity(Vector3(0, 0, 0));
			physics->SetVelocity(Vector3(0, 0, 0));

			physics->SetEnableCollisionResponse(false);
			physics->SetMovable(false);
			physics->SetVelocity(Vector3(0, 0, 0));
			physics->SetGravityUpdate(false);

			state = ACTIVE;
			spriteRotationSpeed = Math::DegreeToRadian(140);
			rangeCheckMulti = 6.0;
		}
	}
	else if (aliveTimer >= 2.4 && state == ACTIVE)
	{
		state = SCALINGDOWN;
	}
	if (state == SCALINGDOWN)
	{
		scale -= normalisedScale * scaleSpeed * dt;
		if (scale.x < 0.1 || scale.y < 0.1)
		{
			scale.x = 0.1;
			scale.y = 0.1;
			state = SUICIDE;
		}
	}
	else if (state == SUICIDE)
	{
		scale += normalisedScale * scaleSpeed * dt * 5;
		spriteRotationSpeed = Math::DegreeToRadian(30);
		this->enableCollision = true;
		this->rangeCheckMulti = 2.2;
		if (scale.x > 7 && scale.y > 7)
		{
			scale.x = 7;
			scale.y = 7;
			physics->SetMovable(true);//Delete properly
			dead = true;
		}
	}

	this->angle += spriteRotationSpeed * dt;
	if (angle > 2 * Math::PI)
	{
		angle = 0;
	}
}

void BlackHoleBullet::CollidedWith(GameObject* go)
{
	if (state == SUICIDE)
	{
		if (go->type == GameObject::GO_TILE)
		{
			DEBUG_MSG("Found tile");
			if (scale.x >= 7 && scale.y >= 7)
			{
				if (go->IsResponable())
				{
					go->SetTemporaryDisable(4);
				}
				else if (go->IsDamagableByExplosive())
				{
					go->dead = true;
				}
			}
		}
		
	}
	if (state == ACTIVE)
	{
		if (go->type == GO_ENEMY || go->type == GO_BULLET && go != this)
		{
			DEBUG_MSG("FOUND");
			if (go->pos.x < 0) go->pos.x *= -1;
			Vector3 dis = this->pos - go->pos;
			if (go->type == GO_BULLET)
			{
				Bullet* poten_bul = dynamic_cast<Bullet*>(go);
				if (poten_bul->GetBulletType() == BULLET_TYPE::BLACKHOLE) // Dont suck in other Blackholes
				{
					return;
				}
			}
			if (!dis.IsZero())
			{
				DEBUG_MSG("DIS length square" << dis.LengthSquared());
				dis.z = 0;
				Vector3 dis_N = dis.Normalized();
				if (dis.LengthSquared() > 1 * 1 && dis.LengthSquared() <= 140 * 140)
				{
					go->physics->SetVelocity(Vector3(0, 0, 0));
					DEBUG_MSG("PRE " << go->pos << "\n" << "bh pos " << pos);
					go->pos += dis_N * 14 * deltaTime;
					go->physics->pos += dis_N * 14 * deltaTime;
					DEBUG_MSG("GO MOVING INTO BH pos " << go->pos << "\n" << "bh pos " << pos);
				}

				if (dis.LengthSquared() <= 8 * 8)
				{
					if (go->currentHP > 0)
					{
						go->currentHP -= 10 * deltaTime;
					}
					else if (go->type == GO_BULLET)
					{
						go->dead = true;
					}
				}
			}

		}
	}
}

GameObject* BlackHoleBullet::Clone()
{
	BlackHoleBullet* cloned = new BlackHoleBullet(this->mesh, this->geoTypeID, this->maxScale, this->attachedPlayer);
	cloned->scaleSpeed = this->scaleSpeed;
	cloned->normalisedScale = normalisedScale;
	cloned->maxScale = maxScale;
	cloned->spriteRotation = spriteRotation;
	cloned->spriteRotationSpeed = spriteRotationSpeed;


	//GameObject::CloneValues(this, cloned);
	return cloned;
}

int BlackHoleBullet::GetShotXDirection()
{
	return (thrownDirection == RIGHT ? 1 : -1);
}
