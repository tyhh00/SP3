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

	//PLASMA BULLET FUNCTIONALITY
	state = BLACKHOLEBULLET_STATE::ROLLING;

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

	//Attach pos of bullet to player
	if (attachedPlayer != nullptr)
	{
		this->pos.z = attachedPlayer->pos.z+1; //Set it infront of player
	}

	if (aliveTimer < 0.5)
	{
		this->scale += normalisedScale * scaleSpeed * dt; 
	}
	else if (aliveTimer >= 0.5 && state == ROLLING
		&& physics->GetVelocity().x < 1)
	{
		DEBUG_MSG("Rolling");
		physics->SetInelasticity(0.99f);
		physics->SetIsBouncable(false);
		physics->SetGravity(Vector3(0, 0, 0));
		physics->SetVelocity(Vector3(0, 0, 0));
		state = ACTIVE;
		spriteRotationSpeed = Math::DegreeToRadian(120);
	}
	else if (aliveTimer >= 3.0)
	{
		state = SCALINGDOWN;
		scale -= normalisedScale * scaleSpeed * dt;
		if (scale.x < 0.1 || scale.y < 0.1)
		{
			scale.x = 0.1;
			scale.y = 0.1;
			dead = true;
			DEBUG_MSG("DEactivating blackhole");
		}
	}
	/*float angle = Math::RadianToDegree(atan2(physics->GetNormal().y, physics->GetNormal().x));
	angle += spriteRotationSpeed * dt;
	physics->SetNormal(Vector3(cosf(angle), sinf(angle)));*/
}

void BlackHoleBullet::CollidedWith(GameObject* go)
{
	
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
