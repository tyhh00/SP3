
#include "Player.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

void Player::Init()
{
	AkeyDown = false;
	DkeyDown = false;
	spaceKeyDown = false;


	physics = new Physics(RECTANGLE, pos, scale);
	physics->SetMass(5);
	physics->SetMovable(true);
	physics->SetNormal(Vector3(1, 0, 0));

	speed = 1000.0f;
	jump_force = 6000.0f;

	animatedSprites = MeshBuilder::GenerateSpriteAnimation(4, 3, 2.0f, 2.0f);
	animatedSprites->AddAnimation("idle", 0, 1);
	animatedSprites->AddAnimation("right", 6, 8);
	animatedSprites->AddAnimation("left", 3, 5);
	animatedSprites->PlayAnimation("idle", -1, 1.0f);
	mesh = animatedSprites;
	mesh->textureID = LoadTGA("Image/girlsprite.tga");
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
		physics->SetVelocity(Vector3(physics->GetVelocity().x + speed * dt, physics->GetVelocity().y, physics->GetVelocity().z));
	}
	else if (!AkeyDown && Application::IsKeyPressed('A'))
	{
		AkeyDown = true;
		std::cout << "A Key Pressed" << std::endl;
		animatedSprites->PlayAnimation("left", -1, 1.0f);
		physics->SetVelocity(Vector3(physics->GetVelocity().x - speed * dt, physics->GetVelocity().y, physics->GetVelocity().z));
	}
	if (DkeyDown && !Application::IsKeyPressed('D'))
	{
		DkeyDown = false;
		std::cout << "D Key Released" << std::endl;
		if (animatedSprites->GetCurrentAnimation() == "right")
		{
			animatedSprites->PlayAnimation("idle", -1, 1.0f);
		}
		physics->SetVelocity(Vector3(physics->GetVelocity().x - speed * dt, physics->GetVelocity().y, physics->GetVelocity().z));
	}
	else if (!DkeyDown && Application::IsKeyPressed('D'))
	{
		DkeyDown = true;
		std::cout << "D Key Pressed" << std::endl;
		animatedSprites->PlayAnimation("right", -1, 1.0f);
		physics->SetVelocity(Vector3(physics->GetVelocity().x + speed * dt, physics->GetVelocity().y, physics->GetVelocity().z));
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
		physics->SetVelocity(Vector3(physics->GetVelocity().x, physics->GetVelocity().y + accel_amt * dt, physics->GetVelocity().z));
	}

}
