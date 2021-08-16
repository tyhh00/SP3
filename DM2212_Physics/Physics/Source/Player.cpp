
#include "Player.h"
#include "Application.h"

void Player::Init()
{
	AkeyDown = false;
	DkeyDown = false;
}

void Player::Update(double dt)
{
	float walking_vel = 20.0f;
	if (AkeyDown && !Application::IsKeyPressed('A'))
	{
		AkeyDown = false;
		std::cout << "A Key Released" << std::endl;
		vel.x += walking_vel;
	}
	else if (!AkeyDown && Application::IsKeyPressed('A'))
	{
		AkeyDown = true;
		std::cout << "A Key Pressed" << std::endl;
		vel.x -= walking_vel;
	}
	if (DkeyDown && !Application::IsKeyPressed('D'))
	{
		DkeyDown = false;
		std::cout << "D Key Released" << std::endl;
		vel.x -= walking_vel;
	}
	else if (!DkeyDown && Application::IsKeyPressed('D'))
	{
		DkeyDown = true;
		std::cout << "D Key Pressed" << std::endl;
		vel.x += walking_vel;
	}

	pos += vel * dt;
}
