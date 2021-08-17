#include "Keyboard.h"
#include "Application.h"
#include <iostream>

Keyboard::Keyboard()
{
	for (int i = 0; i < MAX_KEYS; i++)
	{
		prevStatus[i] = false;
		currStatus[i] = false;
	}
}

void Keyboard::Init()
{
}

void Keyboard::Update(double dt)
{
	std::cout << "kb updating" << std::endl;
	for (int i = 0; i < MAX_KEYS; i++)
	{
		prevStatus[i] = currStatus[i];
		if ((GetAsyncKeyState(i) & 0x8001) != 0)
		{
			currStatus[i] = true;
		}
		else
		{
			currStatus[i] = false;
		}
	}
}

void Keyboard::PostUpdate(double dt)
{
	for (int i = 0; i < MAX_KEYS; i++)
	{
		prevStatus[i] = currStatus[i];
	}
}

bool Keyboard::IsKeyPressed(unsigned short key)
{
	return (!prevStatus[key] && currStatus[key]);
}

bool Keyboard::IsKeyDown(unsigned short key)
{
	return currStatus[key];
}

bool Keyboard::IsKeyReleased(unsigned short key)
{
	return (prevStatus[key] && !currStatus[key]);
}
