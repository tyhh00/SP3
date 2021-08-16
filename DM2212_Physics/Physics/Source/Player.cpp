
#include "Player.h"
#include "Application.h"

void Player::Init()
{
	AkeyDown = false;
	DkeyDown = false;

	speed = 1000.0f;
}

void Player::Update(double dt)
{
	if (AkeyDown && !Application::IsKeyPressed('A'))
	{
		AkeyDown = false;
		std::cout << "A Key Released" << std::endl;
		vel.x += speed * dt;
	}
	else if (!AkeyDown && Application::IsKeyPressed('A'))
	{
		AkeyDown = true;
		std::cout << "A Key Pressed" << std::endl;
		vel.x -= speed * dt;
	}
	if (DkeyDown && !Application::IsKeyPressed('D'))
	{
		DkeyDown = false;
		std::cout << "D Key Released" << std::endl;
		vel.x -= speed * dt;
	}
	else if (!DkeyDown && Application::IsKeyPressed('D'))
	{
		DkeyDown = true;
		std::cout << "D Key Pressed" << std::endl;
		vel.x += speed * dt;
	}

	pos += vel * dt;
}
