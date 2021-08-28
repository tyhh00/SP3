
#include "Prowler.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Flashlight.h"
#include "../Source/SoundController/SoundController.h"

Prowler::Prowler() : Enemy(JG_PROWLER)
{
}

Prowler::~Prowler()
{
}

void Prowler::Init(SceneBase* scene, Inventory* inventory, Player* _player, GameObjectManager* _goManager)
{
	this->scene = scene;
	this->inventory = inventory;
	this->goManager = _goManager;
	player = _player;

	state = IDLE;

	movement_speed = 20.0f;
	activeRange = 80.f;
	attackRange = 10.f;
	defendRange = 40.f;
	shootTimer = 2;

	currentHP = 7;
	maxHP = 7; // IN SECONDS
	
	attackAnimationTimer = 0;
	stunnedTimer = 0;
	spawnCooldownTimer = 0;
	spawnAnimationTimer = 0;

	readyToSpawnMonkey = true;
	readyToAttack = true;

	physics->SetMovable(true);
	physics->SetEnableCollisionResponse(true);
	physics->SetGravity(Vector3(0,-98.f,0));
	this->AddToResponseWhitelist(GO_PLAYER);

	animatedSprites = MeshBuilder::GenerateSpriteAnimation(20, 9, 5.0f, 5.0f);
	animatedSprites->AddAnimation("idleRight", 0, 4);
	animatedSprites->AddAnimation("idleLeft", 90, 94);
	animatedSprites->AddAnimation("runRight", 9, 16);
	animatedSprites->AddAnimation("runLeft", 99, 106);
	animatedSprites->AddAnimation("attackRight", 27, 35);
	animatedSprites->AddAnimation("attackLeft", 117, 126);
	animatedSprites->AddAnimation("spawnRight", 45, 50);
	animatedSprites->AddAnimation("spawnLeft", 136, 141);
	animatedSprites->AddAnimation("stunnedRight", 81, 86);
	animatedSprites->AddAnimation("stunnedLeft", 171, 176);

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
		if ((player->pos - pos).Length() < activeRange)
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
		physics->SetVelocity(Vector3((player->pos - pos).Normalized().x * movement_speed, physics->GetVelocity().y, physics->GetVelocity().z));
		if ((player->pos - pos).Length() < attackRange)
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
		//CSoundController::GetInstance()->PlaySoundByID(PROWLER_FOOTSTEPS);
		break;
	case ATTACK:
	{
		if ((player->pos - pos).Length() < attackRange * 0.5f && readyToAttack)
		{
			player->currentHP -= ATTACK_DAMAGE;
			readyToAttack = false;
		}
		CSoundController::GetInstance()->PlaySoundByID(PROWLER_ATTACK);

		attackAnimationTimer += dt;
		if (attackAnimationTimer > 1.f)
		{
			attackAnimationTimer = 0;
			state = DEFEND;
			readyToAttack = true;
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
		if ((player->pos - pos).Length() < defendRange)
		{
			if (!(player->pos - pos).IsZero())
				physics->SetVelocity(Vector3(-(player->pos - pos).Normalized().x * movement_speed * 1.5f, physics->GetVelocity().y, physics->GetVelocity().z));
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
		physics->SetVelocity(Vector3(0, physics->GetVelocity().y, physics->GetVelocity().z));
		if (spawnAnimationTimer > 1)
		{
			//get the offset of the monkey to be spawned towards the player
			if ((player->pos - pos).IsZero())
				break;
			float x = (player->pos - pos).Normalized().x;
			float offset = 0;

			if (x > 0)
			{
				offset = 10;
			}
			else if (x < 0)
			{
				offset = -10;
			}
			CSoundController::GetInstance()->PlaySoundByID(PROWLER_SPAWN_MINION);

			Monkey* monkey = new Monkey();

			monkey->active = true;
			monkey->scale = Vector3(3, 3, 3);
			monkey->pos = Vector3(pos.x + offset, pos.y, pos.z);
			monkey->physics = physics->Clone();
			monkey->physics->SetInelasticity(0.99f);
			monkey->physics->SetIsBouncable(false);
			monkey->physics->SetGravity(Vector3(0, 0, 0));
			monkey->Init(this->scene, goManager, inventory, player->pos, new BulletSpawner(goManager, new BananaBullet(Vector3(2, 2, 2), monkey, 30)));

			monkey->AddBottomSprite();
			monkey->bottomSprite->mesh = this->scene->GetMeshList(SceneBase::GEO_WALL);
			goManager->AddGO(monkey);

			state = DEFEND;
			readyToSpawnMonkey = false;
			spawnAnimationTimer = 0;
		}
		else
			spawnAnimationTimer += dt;

		if (physics->GetVelocity().x > 0)
		{
			animatedSprites->PlayAnimation("spawnRight", 0, 1.0f);
		}
		else if (physics->GetVelocity().x < 0)
		{
			animatedSprites->PlayAnimation("spawnLeft", 0, 1.0f);
		}
		else
		{
			if ((player->pos - pos).IsZero())
				break;
			float x = (player->pos - pos).Normalized().x;
			if (x > 0)
			{
				animatedSprites->PlayAnimation("idleRight", -1, 1.0f);
			}
			else if (x < 0)
			{
				animatedSprites->PlayAnimation("idleLeft", -1, 1.0f);
			}
		}
	}
	break;
	case STUNNED:
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