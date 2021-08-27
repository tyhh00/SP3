
#include "Tumbleweed.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Flashlight.h"

Tumbleweed::Tumbleweed() : Enemy(GY_TUMBLEWEED)
{
}

Tumbleweed::~Tumbleweed()
{
}

void Tumbleweed::Init()
{

	state = ROLL;

	roll_speed = 25.0f;
	roll_timer = 3.0f;

	timeout = 0;

	physics->SetMovable(true);
	physics->SetGravity(Vector3(0, 0, 0));
	animatedSprites = MeshBuilder::GenerateSpriteAnimation(1, 6, 2.0f, 2.0f);

	animatedSprites->AddAnimation("roll", 0, 5);
	animatedSprites->PlayAnimation("roll", -1, 1.0f);
	

	mesh = animatedSprites;
	mesh->textureID = LoadTGA("Image/Tumbleweed_sprite.tga");

}

void Tumbleweed::Update(double dt)
{ 
	switch (state)
	{
	case ROLL:
		if (roll_timer <= 0)
		{
			roll_timer = 3.0f;
			roll_speed *= -1;
		}
		else
		{
			roll_timer -= dt;
		}
		break;
	}

	physics->SetVelocity(Vector3(roll_speed, 0, 0));
	
	animatedSprites->Update(dt);

	if (timeout > 0)
	{
		timeout -= dt;
		if (timeout < 0)
		{
			timeout = 0;
		}
	}
}

void Tumbleweed::CollidedWith(GameObject* go)
{
	if (go->type == GO_PLAYER)
	{
		std::cout << "TUMBLEWEED COLLIDED W PLAYER" << std::endl;
		if (timeout <= 0)
		{
			go->currentHP -= 10;
			timeout = 1.5f;
		}
	}
}
