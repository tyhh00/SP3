
#include "Ghost.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Flashlight.h"

Ghost::Ghost() : Enemy(GY_GHOST)
{
}

Ghost::~Ghost()
{
}

void Ghost::Init(SceneBase* scene, Inventory* inventory, Vector3 &target)
{
	Enemy::Init();
	this->scene = scene;
	this->inventory = inventory;
	playerPos = &target;

	state = INACTIVE;

	haunting_speed = 5.0f;
	hostile_speed = 25.0f;
	rage_speed = 30.0f;
	activeRange = 40.0f;
	hostileRange = 20.0f;
	inactiveRange = 100.0f;

	currentHP = 7;
	maxHP = 7; // IN SECONDS

	state_timer = 0;
	state_interval;

	physics->SetMovable(true);
	physics->SetEnableCollisionResponse(false);
	physics->SetGravity(Vector3(0,0,0));

	animatedSprites = MeshBuilder::GenerateSpriteAnimation(4, 3, 2.0f, 2.0f);
	animatedSprites->AddAnimation("inactive", 9, 11);
	animatedSprites->AddAnimation("right", 6, 8);
	animatedSprites->AddAnimation("left", 3, 5);
	animatedSprites->AddAnimation("mid", 0, 2);

	mesh = animatedSprites;
	mesh->textureID = LoadTGA("Image/ghost_sprite.tga");

	//geoTypeID = mesh->textureID;

	animatedSprites->PlayAnimation("inactive", -1, 1.0f);

}

void Ghost::Update(double dt)
{ 
	mesh->material.kDiffuse.Set(1.0f, 1.0f, 1.0f);
	if (isBeingAttacked())
	{
		mesh->material.kDiffuse.Set(1.0f, 0.5f, 0.5f);
		state_interval = 0;
		state = RAGE;
		currentHP -= dt;
	}

	if (timeout > 0)
	{
		timeout -= dt;
		if (timeout < 0)
		{
			timeout = 0;
		}
	}

	if (currentHP <= 0)
	{
		dead = true;
		return;
	}
	switch (state)
	{
	case INACTIVE:
		if ((*playerPos - pos).Length() < activeRange)
		{
			state = FOLLOWING;
		}
		break;
	case FOLLOWING:
	{
		if ((*playerPos - pos).Length() < hostileRange)
		{
			state = HOSTILE;
			state_interval = 0;
			physics->SetVelocity((*playerPos - pos).Normalized() * hostile_speed);
			break;
		}
		if (isWithinFlashlight())
		{
			state = HIDING;
			physics->SetVelocity(Vector3(0, 0, 0));
			animatedSprites->PlayAnimation("inactive", -1, 1.0f);
			break;
		}

		physics->SetVelocity((*playerPos - pos).Normalized() * haunting_speed);
	}
		break;
	case HIDING:
		if (!isWithinFlashlight())
		{
			state = FOLLOWING;
			break;
		}
		break;
	case WANDER:
		if ((*playerPos - pos).Length() < activeRange)
		{
			state = HOSTILE;
			state_interval = 0;
			physics->SetVelocity((*playerPos - pos).Normalized() * hostile_speed);
			break;
		}
		if (state_timer >= 10.0f)
		{
			state = INACTIVE;
			animatedSprites->PlayAnimation("inactive", -1, 1.0f);
			physics->SetVelocity(Vector3(0, 0, 0));
			break;
		}
		if (state_interval >= 2.0f)
		{
			state_interval = 0;
			Vector3 dir(Math::RandFloatMinMax(-1, 1), Math::RandFloatMinMax(-1, 1), 0);
			physics->SetVelocity(dir.Normalized() * haunting_speed);
		}
		state_timer += dt;
		state_interval += dt;
		break;
	case HOSTILE:
		if ((*playerPos - pos).Length() > activeRange)
		{
			state = WANDER;
			state_timer = 0;
			state_interval = 0;
			physics->SetVelocity(Vector3(0, 0, 0));
			break;
		}
		if (timeout <= 0)
		{
			physics->SetVelocity((*playerPos - pos).Normalized() * hostile_speed);
		}
		else
		{
			physics->SetVelocity((*playerPos - pos).Normalized() * 0);
		}
		break;
	case RAGE:
		if (timeout <= 0)
		{
			physics->SetVelocity((*playerPos - pos).Normalized() * rage_speed);
		}
		else
		{
			physics->SetVelocity((*playerPos - pos).Normalized() * 0);
		}
		break;
	}

	animatedSprites->Update(dt);

	if (physics->GetVelocity().x > 0)
	{
		animatedSprites->PlayAnimation("right", -1, 1.0f);
	}
	else if (physics->GetVelocity().x < 0)
	{
		animatedSprites->PlayAnimation("left", -1, 1.0f);
	}

	
}

void Ghost::CollidedWith(GameObject* go)
{
	if (go->type == GO_PLAYER)
	{
		if (timeout <= 0)
		{
			go->currentHP -= 20;
			timeout = 2.0f;
		}
	}
}

bool Ghost::isWithinFlashlight()
{
	if (inventory->GetCurrentItem() != nullptr)
	{
		if (inventory->GetCurrentItemType() == Item::I_FLASHLIGHT)
		{
			Flashlight* flashlight = dynamic_cast<Flashlight*>(inventory->GetCurrentItem());
			if (flashlight->isWithinLight(pos))
			{
				return true;
			}
		}
	}

	return false;
}

bool Ghost::isBeingAttacked()
{
	if (!isWithinFlashlight())
	{
		return false;
	}
	Flashlight* flashlight = dynamic_cast<Flashlight*>(inventory->GetCurrentItem());
	if (flashlight->isIntensified())
	{
		return true;
	}
	return false;
}
