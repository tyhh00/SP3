#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Apple.h"

Apple::Apple()
{
}

void Apple::Init()
{
}

void Apple::Update(double dt)
{
	if (Application::IsKeyPressed('G'))
	{
		std::cout << "consumed an apple" << std::endl;
	}
}
