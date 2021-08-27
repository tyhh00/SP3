
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

void GrimReaper::Init(SceneBase* scene, GameObjectManager* GOM, Vector3 &target)
{
	this->scene = scene;
	this->goManager = GOM;
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
	currentHP = goManager->GetNumGOsbyType(GO_TOMBSTONE) * 10;
	
	std::cout << "GrimReaper.cpp : CURRENT HP: " << currentHP << std::endl;
	if (currentHP <= 0)
	{
		dead = true;
		return;
	}

	switch (state)
	{
	case INACTIVE:
		state = CHASING;
		break;
	case CHASING:
		if ((pos - *playerPos).Length() < whackRange)
		{
			state = PREWHACKING;
			attack_timer = 0.5;
			cooldown_timer = 0;
			animatedSprites->PlayAnimation("HostileIdle", 0, 0.5f);
			physics->SetVelocity(Vector3(0, 0, 0));
			break;
		}
		physics->SetVelocity((*playerPos - pos).Normalized() * hostile_speed);
		break;
	case PREWHACKING:
		if (attack_timer <= 0)
		{
			state = WHACKING;
			animatedSprites->PlayAnimation("HostileAttack", 0, 1.0f);
			animatedSprites->Reset();
			break;
		}
		attack_timer -= dt;
		break;
	case WHACKING:
		if (attack_timer <= 0)
		{
			state = COOLDOWN;
			cooldown_timer += 1.0f;
			animatedSprites->PlayAnimation("HostileIdle", -1, 1.0f);
			break;
		}
		attack_timer -= dt;
		break;
	case COOLDOWN:
		if (cooldown_timer <= 0)
		{
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

bool GrimReaper::Interact()
{
	if (animatedSprites->GetCurrentAnimation() != "WieldWeapon")
	{
		animatedSprites->PlayAnimation("WieldWeapon", 0, 1.0f);
		animatedSprites->Reset();
		return false;
	}
	else if (animatedSprites->currentFrame == 29)
	{
		return true;
	}
}

void GrimReaper::CollidedWith(GameObject* go)
{
	if (go->type == GO_PLAYER && 
		state == WHACKING && cooldown_timer <= 0)
	{
		//go->currentHP -= 40;
		cooldown_timer += 2.f;
	}
}
