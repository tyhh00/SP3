
#include "Monkey.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Flashlight.h"

Monkey::Monkey() : Enemy(GY_MONKEY)
{
}

Monkey::~Monkey()
{
}

void Monkey::Init(SceneBase* scene, Inventory* inventory, Vector3 &target)
{
	this->scene = scene;
	this->inventory = inventory;
	playerPos = &target;

	state = IDLE;

	movement_speed = 20.0f;
	activeRange = 40.0f;

	currentHP = 7;
	maxHP = 7; // IN SECONDS

	state_timer = 0;
	state_interval;

	physics->SetMovable(true);
	physics->SetEnableCollisionResponse(true);
	physics->SetGravity(Vector3(0,-98.f,0));

	//animatedSprites = MeshBuilder::GenerateSpriteAnimation(4, 3, 2.0f, 2.0f);
	//animatedSprites->AddAnimation("inactive", 9, 11);
	//animatedSprites->AddAnimation("right", 6, 8);
	//animatedSprites->AddAnimation("left", 3, 5);
	//animatedSprites->AddAnimation("mid", 0, 2);

	mesh = MeshBuilder::GenerateQuad("monkey", Color(1, 1, 1), 2.f);
	mesh->textureID = LoadTGA("Image/Tiles/JUNGLE_SNOWMAN.tga");

	//animatedSprites->PlayAnimation("inactive", -1, 1.0f);

}

void Monkey::Update(double dt)
{ 
	mesh->material.kDiffuse.Set(1.0f, 1.0f, 1.0f);
	if (isBeingAttacked())
	{
		mesh->material.kDiffuse.Set(1.0f, 0.5f, 0.5f);
		state_interval = 0;
		state = ATTACK;
		currentHP -= dt;
	}

	if (currentHP <= 0)
	{
		dead = true;
		return;
	}
	switch (state)
	{
	case IDLE:
		if ((*playerPos - pos).Length() < activeRange)
		{
			state = JUMP;
		}
		break;
	case JUMP:
		physics->AddVelocity(Vector3((*playerPos - pos).Normalized().x * 5, 50, 0));
		std::cout << (*playerPos - pos).Normalized().x << std::endl;
		state = ATTACK;
		break;
	case ATTACK:
	{
		if (!physics->GetOnGround())
			break;
		//if ((*playerPos - pos).Length() < 2)
		//{
		//	state_interval = 0;
		//	break;
		//}
		if (playerPos->y - 3 > pos.y)
		{
			state = JUMP;
			break;
		}
		else
			physics->SetVelocity(Vector3((*playerPos - pos).Normalized().x * movement_speed, physics->GetVelocity().y, physics->GetVelocity().z));
	}
	break;
	}

	//animatedSprites->Update(dt);

	//if (physics->GetVelocity().x > 0)
	//{
	//	animatedSprites->PlayAnimation("right", -1, 1.0f);
	//}
	//else if (physics->GetVelocity().x < 0)
	//{
	//	animatedSprites->PlayAnimation("left", -1, 1.0f);
	//}
}

void Monkey::StartAttackCooldown()
{
	state_interval = 2.0f;
}

bool Monkey::isBeingAttacked()
{
	/*Flashlight* flashlight = dynamic_cast<Flashlight*>(inventory->GetCurrentItem());
	if (flashlight->isIntensified())
	{
		return true;
	}*/
	return false;
}