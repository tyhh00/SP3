
#include "Crab.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Flashlight.h"

Crab::Crab() : Enemy(OCEAN_CRAB)
{
}

Crab::~Crab()
{
}

void Crab::Init(Vector3 &target, MOVEMENT_TYPE type)
{
	playerPos = &target;

	state = IDLE;
	mType = type;
	activeRange = 30.0f;
	crabTimer = 10;
	WLARTimer = 3;

	physics->SetMovable(true);

	animatedSprites = MeshBuilder::GenerateSpriteAnimation(8, 7, 2.0f, 2.0f);

	animatedSprites->AddAnimation("right", 3, 5);
	animatedSprites->AddAnimation("left", 1, 2);
	
	tempVel = 0;
	mesh = animatedSprites;
	mesh->textureID = LoadTGA("Image/enemy_crabSprite.tga");

	animatedSprites->PlayAnimation("left", -1, 1.0f);

}

void Crab::Update(double dt)
{ 
	switch (state)
	{
	case IDLE:
		if (crabTimer <= 0)
		{
			state = WLAR;
			if (mType == LAR)
			{
				tempVel = 5;
			}
			crabTimer = 10;
			WLARTimer = 3;
		}
		else
		{
			crabTimer -= dt;
		}
		break;
	case WLAR:
		if (crabTimer <= 0)
		{
			state = IDLE;
			crabTimer = 10;
		}
		else
		{
			updateMType(dt);
			crabTimer -= dt;
		}
		break;
	case ATTACK:
		break;
	case DIE:
		break;
	}

	animatedSprites->Update(dt);

	/*if (physics->GetVelocity().x > 0)
	{
		animatedSprites->PlayAnimation("right", -1, 1.0f);
	}
	else if (physics->GetVelocity().x < 0)
	{
		animatedSprites->PlayAnimation("left", -1, 1.0f);
	}*/

}

void Crab::updateMType(double dt)
{
	switch (mType)
	{
	case LAR:
		if (WLARTimer <= 0)
		{
			std::cout << "stateChanged" << std::endl;
			tempVel *= -1;
			std::cout << "Vel: " << physics->GetVelocity() << std::endl;
			WLARTimer = 3;
		}
		else
		{
			WLARTimer -= dt;
		}
		physics->SetVelocity(Vector3(tempVel, physics->GetVelocity().y, physics->GetVelocity().z));		
		break;
	case EDGE:
		break;
	case JUMP:
		break;
	}
}

void Crab::CollidedWith(GameObject* go) //walk into a wall
{
	switch (go->geoTypeID)
	{
	case SceneBase::GEO_OCEAN_1_MIDRIGHT:
	case SceneBase::GEO_OCEAN_1_MIDLEFT:
	case SceneBase::GEO_LOBBY_FLOOR:
		physics->GetVelocity().x * -1;
		WLARTimer = 3;
		break;
	}
}
