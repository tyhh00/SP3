
#include "Player.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

void Player::Init()
{
	AkeyDown = false;
	DkeyDown = false;
	spaceKeyDown = false;

	physics->SetMass(1);
	physics->SetMovable(true);

	speed = 1000.0f;
	jump_force = 10000.0f;

	keyboard = Keyboard::GetInstance();

	animatedSprites = MeshBuilder::GenerateSpriteAnimation(4, 3, 2.0f, 2.0f);
	animatedSprites->AddAnimation("idle", 0, 1);
	animatedSprites->AddAnimation("right", 6, 8);
	animatedSprites->AddAnimation("left", 3, 5);
	animatedSprites->PlayAnimation("idle", -1, 1.0f);
	mesh = animatedSprites;
	mesh->textureID = LoadTGA("Image/girlsprite.tga");

	// CHEAT FIX - TBC; LIGHTING NOT WORKING ON SPRITE ANIMATION MESH
	mesh->material.kAmbient.Set(1, 1, 1);

}

void Player::Update(double dt)
{ 
	animatedSprites->Update(dt);

	if (AkeyDown && !Application::IsKeyPressed('A'))
	{
		AkeyDown = false;
		std::cout << "A Key Released" << std::endl;
		if (animatedSprites->GetCurrentAnimation() == "left")
		{
			animatedSprites->PlayAnimation("idle", -1, 1.0f);
		}
		physics->AddVelocity(speed * dt);
	}
	else if (!AkeyDown && Application::IsKeyPressed('A'))
	{
		AkeyDown = true;
		std::cout << "A Key Pressed" << std::endl;
		animatedSprites->PlayAnimation("left", -1, 1.0f);
		physics->AddVelocity(-speed * dt);
	}
	if (DkeyDown && !Application::IsKeyPressed('D'))
	{
		DkeyDown = false;
		std::cout << "D Key Released" << std::endl;
		if (animatedSprites->GetCurrentAnimation() == "right")
		{
			animatedSprites->PlayAnimation("idle", -1, 1.0f);
		}
		physics->AddVelocity(-speed * dt);
	}
	else if (!DkeyDown && Application::IsKeyPressed('D'))
	{
		DkeyDown = true;
		std::cout << "D Key Pressed" << std::endl;
		animatedSprites->PlayAnimation("right", -1, 1.0f);
		physics->AddVelocity(speed * dt);
	}

	if (spaceKeyDown && !Application::IsKeyPressed(VK_SPACE))
	{
		spaceKeyDown = false;
		std::cout << "Space Key Released" << std::endl;
		
	}
	else if (!spaceKeyDown && Application::IsKeyPressed(VK_SPACE) 
		&& physics->GetOnGround())
	{
		spaceKeyDown = true;
		std::cout << "Space Key Pressed" << std::endl;
		float accel_amt = jump_force / physics->GetMass();
		physics->AddVelocity(Vector3(0, physics->GetVelocity().y + accel_amt * dt, 0));
	}

}
