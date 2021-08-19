
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
	jump_force = 5000.0f;

	keyboard = Keyboard::GetInstance();

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

	//if (AkeyDown && !Application::IsKeyPressed('A'))
	//{
	//	AkeyDown = false;
	//	std::cout << "A Key Released" << std::endl;
	//	if (animatedSprites->GetCurrentAnimation() == "left")
	//	{
	//		animatedSprites->PlayAnimation("idle", -1, 1.0f);
	//	}
	//	physics->AddVelocity(speed * dt);
	//}
	//else if (!AkeyDown && Application::IsKeyPressed('A'))
	//{
	//	AkeyDown = true;
	//	std::cout << "A Key Pressed" << std::endl;
	//	animatedSprites->PlayAnimation("left", -1, 1.0f);
	//	physics->AddVelocity(-speed * dt);
	//}
	//if (DkeyDown && !Application::IsKeyPressed('D'))
	//{
	//	DkeyDown = false;
	//	std::cout << "D Key Released" << std::endl;
	//	if (animatedSprites->GetCurrentAnimation() == "right")
	//	{
	//		animatedSprites->PlayAnimation("idle", -1, 1.0f);
	//	}
	//	physics->AddVelocity(-speed * dt);
	//}
	//else if (!DkeyDown && Application::IsKeyPressed('D'))
	//{
	//	DkeyDown = true;
	//	std::cout << "D Key Pressed" << std::endl;
	//	animatedSprites->PlayAnimation("right", -1, 1.0f);
	//	physics->AddVelocity(speed * dt);
	//}

	//temp keyboard controls
	if (keyboard->IsKeyDown('A'))
		physics->SetVelocity(Vector3(-speed * dt, physics->GetVelocity().y, physics->GetVelocity().z));
	else if (keyboard->IsKeyDown('D'))
		physics->SetVelocity(Vector3(speed * dt, physics->GetVelocity().y, physics->GetVelocity().z));
	else
		physics->SetVelocity(Vector3(0, physics->GetVelocity().y, physics->GetVelocity().z));

	//animations based on x velocity
	if (physics->GetVelocity().x > 1)
		animatedSprites->PlayAnimation("right", -1, 1.0f);
	else if (physics->GetVelocity().x < -1)
		animatedSprites->PlayAnimation("left", -1, 1.0f);
	else
		animatedSprites->PlayAnimation("idle", -1, 1.0f);

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

	if (keyboard->IsKeyPressed('Q'))
	{
		if (!this->physics->GetVelocity().IsZero() && !isDashing)
			isDashing = true;
	}

	//if in dashing phase
	if (isDashing)
	{
		//disable physics gravity update
		this->physics->SetEnableUpdate(false);

		//disable collision
		this->enableCollision = false;

		//start dash timer
		dashTimer += dt;

		//check if dash time is over
		if (dashTimer > 0.25f)
		{
			//reset dash timer, is dashing and physics update
			dashTimer = 0;
			isDashing = false;
			this->physics->SetEnableUpdate(true);
			this->enableCollision = true;
		}

		//double check if player vel is not zero if not will have divide by zero error in normalized()
		if (!this->physics->GetVelocity().IsZero())
		{
			//get and normalize the player vel  and find out the dash dir
			Vector3 dir = this->physics->GetVelocity().Normalized() * speed * speed * dt;

			//add the dash dir to the player's vel
			this->physics->AddVelocity(Vector3(dir.x, 0, 0));
		}
	}

	//set player's max vel speed
	if (this->physics->GetVelocity().Length() > 100)
		this->physics->SetVelocity(this->physics->GetVelocity().Normalized() * 100);
}

void Player::CollidedWith(GameObject* go)
{
	switch (go->geoTypeID)
	{
	default:
		break;
	}
}
