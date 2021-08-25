
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

void Crab::Init(SceneBase* scene, Inventory* inventory, Vector3 &target)
{
	this->scene = scene;
	this->inventory = inventory;
	playerPos = &target;

	state = INACTIVE;


	roll_speed = 25.0f;
	activeRange = 30.0f;

	physics->SetMovable(true);

	animatedSprites = MeshBuilder::GenerateSpriteAnimation(2, 3, 2.0f, 2.0f);

	animatedSprites->AddAnimation("right", 3, 5);
	animatedSprites->AddAnimation("left", 1, 2);
	

	mesh = animatedSprites;
	mesh->textureID = LoadTGA("Image/enemy_crabSprite.tga");

	animatedSprites->PlayAnimation("left", -1, 1.0f);

}

void Crab::Update(double dt)
{ 
	switch (state)
	{
	case INACTIVE:
		break;
	case ROLL:
		// SET VEL etc
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
