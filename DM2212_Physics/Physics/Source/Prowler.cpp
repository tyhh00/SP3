
#include "Prowler.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Flashlight.h"

Prowler::Prowler() : Enemy(JG_PROWLER)
{
}

Prowler::~Prowler()
{
}

void Prowler::Init(SceneBase* scene, Inventory* inventory, Vector3 &target, GameObjectManager* _goManager)
{
	this->scene = scene;
	this->inventory = inventory;
	this->goManager = _goManager;
	playerPos = &target;

	state = IDLE;

	movement_speed = 20.0f;
	activeRange = 40.0f;
	attackRange = 10.f;
	defendRange = 30.f;
	shootTimer = 2;

	currentHP = 7;
	maxHP = 7; // IN SECONDS
	
	attackTimer = 0;
	stunnedTimer = 0;
	spawnCooldownTimer = 0;

	readyToSpawnMonkey = true;

	physics->SetMovable(true);
	physics->SetEnableCollisionResponse(true);
	physics->SetGravity(Vector3(0,-98.f,0));

	animatedSprites = MeshBuilder::GenerateSpriteAnimation(20, 9, 5.0f, 5.0f);
	animatedSprites->AddAnimation("idleRight", 0, 4);
	animatedSprites->AddAnimation("idleLeft", 90, 94);
	animatedSprites->AddAnimation("runRight", 9, 16);
	animatedSprites->AddAnimation("runLeft", 99, 106);
	animatedSprites->AddAnimation("attackRight", 27, 35);
	animatedSprites->AddAnimation("attackLeft", 117, 126);
	animatedSprites->AddAnimation("stunnedRight", 45, 50);
	animatedSprites->AddAnimation("stunnedLeft", 136, 141);

	mesh = animatedSprites;
	mesh->textureID = LoadTGA("Image/prowler.tga");

	animatedSprites->PlayAnimation("idle", -1, 1.0f);
}

void Prowler::Update(double dt)
{ 
	if (currentHP <= 0)
	{
		dead = true;
		return;
	}
	if (!readyToSpawnMonkey)
	{
		if (spawnCooldownTimer > 15)
		{
			readyToSpawnMonkey = true;
			spawnCooldownTimer = 0;
		}
		spawnCooldownTimer += dt;
	}

	switch (state)
	{
	case IDLE:
		std::cout << "IDLE" << std::endl;
		if ((*playerPos - pos).Length() < activeRange)
		{
			state = CHASE;
			break;
		}
		if (physics->GetVelocity().x > 0)
		{
			animatedSprites->PlayAnimation("idleRight", -1, 1.0f);
		}
		else if (physics->GetVelocity().x < 0)
		{
			animatedSprites->PlayAnimation("idleLeft", -1, 1.0f);
		}
		break;
	case CHASE:
		std::cout << "CHASE" << std::endl;
		physics->SetVelocity(Vector3((*playerPos - pos).Normalized().x * movement_speed, physics->GetVelocity().y, physics->GetVelocity().z));
		if ((*playerPos - pos).Length() < attackRange)
		{
			state = ATTACK;
			animatedSprites->Reset();
			break;
		}
		if (physics->GetVelocity().x > 0)
		{
			animatedSprites->PlayAnimation("runRight", -1, 1.0f);
		}
		else if (physics->GetVelocity().x < 0)
		{
			animatedSprites->PlayAnimation("runLeft", -1, 1.0f);
		}
		break;
	case ATTACK:
	{
		std::cout << "ATTACK" << std::endl;
		attackTimer += dt;
		if (attackTimer > 1.f)
		{
			attackTimer = 0;
			state = DEFEND;
			physics->SetVelocity(Vector3(0, physics->GetVelocity().y, physics->GetVelocity().z));
			break;
		}
		if (physics->GetVelocity().x > 0)
		{
			animatedSprites->PlayAnimation("attackRight", 0, 1.0f);
		}
		else if (physics->GetVelocity().x < 0)
		{
			animatedSprites->PlayAnimation("attackLeft", 0, 1.0f);
		}
		break;
	}
	case DEFEND:
	{
		std::cout << "DEFEND" << std::endl;
		if ((*playerPos - pos).Length() < defendRange)
		{
			physics->SetVelocity(Vector3(-(*playerPos - pos).Normalized().x * movement_speed * 1.5f, physics->GetVelocity().y, physics->GetVelocity().z));
		}
		else
		{
			if (readyToSpawnMonkey)
				state = SPAWN;
			else
				state = IDLE;
		}
		if (physics->GetVelocity().x > 0)
		{
			animatedSprites->PlayAnimation("runRight", -1, 1.0f);
		}
		else if (physics->GetVelocity().x < 0)
		{
			animatedSprites->PlayAnimation("runLeft", -1, 1.0f);
		}
		break;
	}
	case SPAWN:
	{
		std::cout << "SPAWN" << std::endl;
		Monkey* monkey = new Monkey();

		monkey->active = true;
		monkey->scale = Vector3(3, 3, 3);
		monkey->pos = pos;
		monkey->physics = physics->Clone();
		monkey->physics->SetInelasticity(0.99f);
		monkey->physics->SetIsBouncable(false);
		monkey->physics->SetGravity(Vector3(0, 0, 0));
		monkey->Init(this->scene, inventory, *playerPos, new Pistol(goManager, new LightBullet(Vector3(2, 2, 2), monkey, 100), this->scene->GetMeshList(SceneBase::GEO_WALL)));

		monkey->AddBottomSprite();
		monkey->bottomSprite->mesh = this->scene->GetMeshList(SceneBase::GEO_WALL);
		goManager->AddGO(monkey);
		state = DEFEND;
		readyToSpawnMonkey = false;
	}
	break;
	case STUNNED:
		std::cout << "STUNNED" << std::endl;
		stunnedTimer += dt;
		if (stunnedTimer > 3)
		{
			stunnedTimer = 0;
			state = IDLE;
			break;
		}
		if (physics->GetVelocity().x > 0)
		{
			animatedSprites->PlayAnimation("stunnedRight", -1, 1.0f);
		}
		else if (physics->GetVelocity().x < 0)
		{
			animatedSprites->PlayAnimation("stunnedLeft", -1, 1.0f);
		}
		break;
	}

	animatedSprites->Update(dt);
}

bool Prowler::isBeingAttacked()
{
	/*Flashlight* flashlight = dynamic_cast<Flashlight*>(inventory->GetCurrentItem());
	if (flashlight->isIntensified())
	{
		return true;
	}*/
	return false;
}