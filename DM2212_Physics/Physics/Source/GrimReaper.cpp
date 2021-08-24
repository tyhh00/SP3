
#include "GrimReaper.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Flashlight.h"

GrimReaper::GrimReaper() : Enemy(GY_GRIMREAPER)
{
}

GrimReaper::~GrimReaper()
{
}

void GrimReaper::Init(SceneBase* scene, Inventory* inventory, Vector3 &target)
{
	this->scene = scene;
	this->inventory = inventory;
	playerPos = &target;

	state = INACTIVE;

	idle_speed = 10.0f;
	hostile_speed = 25.0f;

	activeRange = 30.0f;
	whackRange = 5.0f;

	attack_timer = 0;
	cooldown_timer = 0;

	physics->SetMovable(true);
	physics->SetEnableCollisionResponse(false);
	physics->SetGravity(Vector3(0, 0, 0));

	animatedSprites = MeshBuilder::GenerateSpriteAnimation(7, 10, 2.0f, 2.0f);
	animatedSprites->AddAnimation("PassiveIdle", 0, 4);
	animatedSprites->AddAnimation("PassiveRunning", 10, 17);
	animatedSprites->AddAnimation("WieldWeapon", 20, 29);
	animatedSprites->AddAnimation("HosileIdle", 30, 34);
	animatedSprites->AddAnimation("HostileRunning", 40, 47);
	animatedSprites->AddAnimation("HostileAttack", 50, 59);
	animatedSprites->AddAnimation("HolsterWeapon", 60, 69);
	
	mesh = animatedSprites;
	mesh->textureID = LoadTGA("Image/GrimReaperSprite.tga");

	animatedSprites->PlayAnimation("HostileIdle", -1, 1.0f);

}

void GrimReaper::Update(double dt)
{ 
	switch (state)
	{
	case INACTIVE:
		state = CHASING;
		break;
	case CHASING:
		std::cout << "CHASING" << std::endl;
		if ((pos - *playerPos).Length() < whackRange)
		{
			std::cout << "//PLAYER WITHIN RANGE, ENTERING WHACKING STATE" << std::endl;
			state = WHACKING;
			attack_timer = 1.0;
			cooldown_timer = 0;
			animatedSprites->PlayAnimation("HostileAttack", 0, 1.0f);
			physics->SetVelocity(Vector3(0, 0, 0));
			break;
		}
		physics->SetVelocity((*playerPos - pos).Normalized() * hostile_speed);
		break;
	case WHACKING:
		std::cout << "WHACKING" << std::endl;
		if (attack_timer <= 0)
		{
			state = COOLDOWN;
			cooldown_timer += 1.0f;
			std::cout << "//FINISHED WHACKING, ENTERING COOLDOWN STATE NOW" << std::endl;
			std::cout << "Current Cooldown Time: " << cooldown_timer << std::endl;
			animatedSprites->PlayAnimation("HostileIdle", -1, 1.0f);
			break;
		}
		attack_timer -= dt;
		break;
	case COOLDOWN:
		std::cout << "COOLDOWN" << std::endl;
		if (cooldown_timer <= 0)
		{
			std::cout << "//COOLDOWN END, ENTERING CHASING STATE NOW" << std::endl;
			state = CHASING;
			cooldown_timer = 0;
			animatedSprites->PlayAnimation("HostileRunning", -1, 1.0f);
			break;
		}
		cooldown_timer -= dt;
		break;
	
	}

	animatedSprites->Update(dt);

}

void GrimReaper::CollidedWith(GameObject* go)
{
	if (go->type == GO_PLAYER && 
		state == WHACKING && cooldown_timer <= 0)
	{
		//go->currentHP -= 40;
		std::cout << "//// PLAYER IS WHACKED. SET COOLDOWN." << std::endl;
		cooldown_timer += 2.f;
	}
}
