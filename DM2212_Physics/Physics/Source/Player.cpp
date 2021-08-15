
#include "Player.h"
#include "Application.h"

void Player::Init()
{
	SPEED = 25.0f;
}

void Player::Update(double dt)
{
	if (Application::IsKeyPressed('A'))
	{
		pos.x -= SPEED * dt;
	}
	else if (Application::IsKeyPressed('D'))
	{
		pos.x += SPEED * dt;
	}
	else if (Application::IsKeyPressed('W'))
	{
		pos.y += SPEED * dt;
	}
	else if (Application::IsKeyPressed('S'))
	{
		pos.y -= SPEED * dt;
	}
}
