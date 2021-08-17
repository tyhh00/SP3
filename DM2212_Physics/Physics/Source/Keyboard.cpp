#include "Keyboard.h"
bool Keyboard::prevStatus[];
bool Keyboard::currStatus[];

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
	for (int i = 0; i < MAX_KEYS; i++)
	{
		prevStatus[i] = currStatus[i];
		currStatus[i] = false;
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
