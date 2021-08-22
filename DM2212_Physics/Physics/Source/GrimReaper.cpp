
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

	idle_speed = 15.0f;
	hostile_speed = 30.0f;

	activeRange = 30.0f;

	physics->SetMovable(true);

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

	animatedSprites->PlayAnimation("PassiveIdle", -1, 1.0f);

}

void GrimReaper::Update(double dt)
{ 
	switch (state)
	{
	case INACTIVE:
		break;
	
	}

	animatedSprites->Update(dt);


}
